#include "ircserv.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void	Server::_setNickname(Client *cli, std::vector<std::string> params)
{
	if (cli->getStatus() == DONE)
	{
		std::string oldNick = cli->getNickName();
    	cli->setNickName(params[0]);
	    std::string nickChangeMessage = ":" + oldNick + " NICK :" + params[0] + "\r\n";
		infoAllServerClients(nickChangeMessage);
	}
	else if (cli->getStatus() != NICK)
	{
		cli->addBuffer("451 * : USER not reregistred ( need PASS )\r\n");
	}
	else if (params.size() == 0)
	{
		cli->addBuffer(std::string("431 * :No set a NickName\r\n"));
	}
	else if (getIsNickNameInUse(params[0]))
	{
		cli->addBuffer(std::string("433 * " + params[0] + " :Nickname is already in use\r\n"));

	}
	else
	{
		std::cout << "NickName saved: " + params[0] << std::endl;
		cli->setNickName(params[0]);
		if (cli->getStatus() == NICK)
			cli->nextStatus();
	}
}

void	Server::_setUser(Client *cli, std::vector<std::string> params)
{
	// std::cout << params.size() << std::endl;
	if (cli->getStatus() != USER)
	{
		cli->addBuffer("462 * : USER not reregistred ( need PASS and NICK )\r\n");
	}
	else
	{
		cli->setUserName(params[1]);
		std::cout << "User saved: " << params[0] << std::endl;
		handleConnection(cli);
		cli->nextStatus();
		std::vector< std::string > chanelUserName;
		chanelUserName.push_back( "#" + params[0]);
		chanelUserName.push_back("1");
		_handleJoin( cli, chanelUserName , 1);
	}
}

std::vector<std::string> splitString(const std::string& input, char delimiter)
{
    std::istringstream stream(input);
    std::string token;
    std::vector<std::string> ret;

    while (std::getline(stream, token, delimiter))
        ret.push_back(token);
    
    return ret;
}

void	Server::_handlePrivmsg(Client *cli, std::vector<std::string> params)
{
	if (cli->getStatus() != DONE)
	{
		cli->addBuffer(std::string("451 * :PRIVMSG-You have not registered \r\n"));
	}
	else if (params.size() < 2)
	{
		cli->addBuffer(ERR_PARAM461);
		return;
	}

    std::string target = params[0]; // El destinatario (cliente o canal)
    std::string message = params[1]; // El mensaje (puedes unir los params si hay espacios)
	std::string token;
	std::vector<std::string> tokens;
	std::istringstream iss( target );
	while (std::getline(iss, token, ','))
	{
		tokens.push_back(token);
	}
	for( size_t i = 0; i < tokens.size(); i++)
	{
		std::string name = tokens[i];
		Client *client = getClientNickName(cli->getNickName());
		if (!client)
		{
        	std::cerr << "Error: No clients to send" << std::endl;
        	break ;
    	}
		if (name[0] == '#')
		{
			if (Channel *channel = findChannel(name))
			{
				if (channel->isClient(cli))
				{
					sendMsgToChannel( client->getFd(), std::string(":" + client->getNickName() + " PRIVMSG " + name + " :" + params[1] + " \r\n"), channel );
				}
				else
				{
					client->addBuffer(std::string("404 " + client->getNickName() + " " + name + " :Cannot send to channel \r\n"));
				}
			}
			else
			{
				client->addBuffer(std::string("403 " + client->getNickName() + " " + name + " :No such channel\r\n"));
			}
		}
	}
}
// /KICK <channel> <user> [<reason>]
void Server::_handleKick(Client *cli, std::vector<std::string> params)
{
    if (params.size() < 2) {
        cli->addBuffer(ERR_PARAM461);
        return;
    }
    std::vector<std::string> channels = splitString(params[0], ',');
    std::vector<std::string> users = splitString(params[1], ',');
    std::string reason = (params.size() >= 3) ? params[2] : "No reason specified";

    for (size_t i = 0; i < channels.size(); i++)
	{
        Channel *channel = findChannel(channels[i]);
        if (!channel) {
            cli->addBuffer(std::string(ERR_NOCHANEL));
            return;
        }
        if (!channel->isAdmin(cli)) {
            cli->addBuffer(std::string(ERR_OPNEEDED));
            return;
        }
        if (i >= users.size()) {
            cli->addBuffer(std::string("441 " + cli->getNickName() + " :No user specified\r\n"));
            return;
        }
        if (!channel->removeClient(cli , users[i])) {
            cli->addBuffer(std::string("441 " + cli->getNickName() + " " + users[i] + " " + channels[i] + " :They aren't on that channel\r\n"));
            return;
        }
		std::string kickMsg = ":" + users[i] + " KICK " + params[0] + " " + cli->getNickName() + " :" + reason + "\r\n";
		infoAllServerClients(kickMsg);
    }
}

void	Server::_handleInvite(Client *cli, std::vector<std::string> params)
{
 	if (params.size() < 2)
    {
        cli->addBuffer(std::string(ERR_PARAM461));
        return;
    }
    Channel *tChannel = findChannel(params[1]);
    if (!tChannel)
    {
        cli->addBuffer(std::string(ERR_NOCHANEL));
        return;
    }
    if (!tChannel->isClient(cli))
    { 
        cli->addBuffer(std::string(ERR_CHANN442));
        return;
    }
    if (!tChannel->isAdmin(cli))
    {
        cli->addBuffer(std::string(ERR_OPNEEDED));
        return;
    }
    Client *target = getClientNickName(params[0]);
    if (!target)
    {
        cli->addBuffer(std::string(ERR_NICKN401));
        return;
    }
    if (tChannel->isClient(target))
    {
        cli->addBuffer(std::string(ERR_INCHA443));
        return;
    }
	tChannel->addInvite(target->getNickName());
    if (target)
	{
        std::string notification = ":" + cli->getNickName() + " INVITE " + target->getNickName() + " :" + tChannel->getName() + "\r\n";
        target->addBuffer(notification);
	}
    cli->addBuffer(std::string("341 * " + params[0] + " " + params[1] + "\r\n"));
}

void	Server::_handleTopic(Client *cli, std::vector<std::string> params)
{
	if (cli->getStatus() != DONE)
	{
		cli->addBuffer(std::string("451 * :TOPIC-You have not registered \r\n"));
		return ;
	}
  	if (params.size() < 1) {
        cli->addBuffer("461 " + cli->getNickName() + " TOPIC :Not enough parameters\r\n");
        return;
    }
	else if (Channel *channel = findChannel(params[0]))
	{
		if (!channel->isClient(cli))
		{
			cli->addBuffer(std::string(ERR_CHANN442));
		}
		else if (params.size() == 1)
		{
    		if (channel->getTopic() == std::string("")) {
        		cli->addBuffer(std::string(ERR_TOPIC331));
    		} else {
        		cli->addBuffer(std::string(MSG_TOPIC332));
    		}
		}
		else if (!channel->istopiclock() || (channel->isAdmin(cli) == 1 && channel->istopiclock()))
		{
			std::string newTopic = params[1];
			channel->setTopic(cli, newTopic);
			std::string topicChangeMsg = ":" + cli->getNickName() + " TOPIC " + params[0] + " :" + channel->getTopic() + "\r\n";
			infoAllServerClients(topicChangeMsg);
		}
		else
		{
			cli->addBuffer(std::string(ERR_OPNEEDED));
		}
	}
	else
	{
		cli->addBuffer(std::string(ERR_NOCHANEL));
	}
}

void	Server::_handleMode(Client *cli, std::vector<std::string> params)
{
	if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{
		if (params.size() < 2){
			std::cout <<ERR << "you need to specify channel and mode"<<std::endl;
			getChannelsList();
			return;
		}
		if (params[0][0] != '#')
			params[0] = "#" + params[0];
		Channel *chann = findChannel(params[0]);
		if (chann == NULL){
			std::cout <<ERR << "channel not found"<<std::endl;
			return;
		}
		std::string type = params[1];
		std::string modes[] = {"+i","+t","+k","+o","+l"};
		int i = 0;
		if (type[0] != '+')
			type = "+" + type;
 		for (i = 0; i < 5; ++i){
			if (modes[i] == type)
				break;
		}
		try{
			switch (i) //DONE
			{
				case 0: //MODE #canal +i
					chann->setInviteOnly(cli);
					cli->addBuffer("Invited permissions changed!\r\n");
					break;
				case 1: //MODE #channel +t
					chann->setTopicAdmin(cli);
					cli->addBuffer("Topic status changed!\r\n");
					break;
				case 2: //MODE #canal +k password
					if (params.size() >= 3)
					{
						chann->setPassword(cli,params[2]);
						cli->addBuffer("Password Changed!\r\n");
					}
					else
						cli->addBuffer("No password provided!\r\n");
					break;
				case 3: //MODE #canal +o <nickname>
					if (params.size() >= 3)
					{
						if (getClientNickName(params[2]) != NULL)
						{
							chann->addAdmin(cli,getClientNickName(params[2]));
							cli->addBuffer("Admin Added!\r\n");
						}
						else
							cli->addBuffer("User not found!\r\n");
					}
					else
						cli->addBuffer("No nickname provided!\r\n");
					break;
				case 4: //MODE #canal +l 50
					if (params.size() >= 3)
					{
						chann->setUserLimit(cli,params[2]);
						cli->addBuffer("User limit Changed!\r\n");
					}
					else
						cli->addBuffer("No user limit provided!\r\n");
					break;
				default:
					chann->showModes();
					std::cout << ERR << "Channel MODE not existent [i, t, k, o, l]" <<std::endl;
					break;
			}
		}catch(std::exception &e){
			std::cout << ERR << e.what() <<std::endl;
		}
	}
}

void	Server::_handlePing(Client *cli, std::vector<std::string> params)
{
    (void) params;
	if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{
    	cli->addBuffer("Ping Marc Pong Albert and javi from munich\r\n");
	}
}

void Server::addChannel(Client *cli, const std::string& channelName, const std::string& password)
{
    Channel newChannel(cli, channelName, password);
    _channels.push_back(newChannel);
}

void Server::_handleJoin(Client *cli, std::vector<std::string> params, bool UserChannel)
{
	if (cli->getStatus() != DONE && !UserChannel) {
		std::cout << "User not registered!" << std::endl; 
		cli->addBuffer("User not registered!\r\n");
	} else {
		// Param format: [#channel1,#channel2 key1,key2]
		std::vector<std::pair<std::string, std::string> > channelKeyPairs;
		std::vector<std::string> channels, keys;
		std::istringstream namechanel(params[0]);
		std::istringstream passchanel(params[1]);
		std::string channIn, keysIn;

		std::getline(namechanel, channIn, ' ');
		std::getline(passchanel, keysIn);

		if (channIn.empty() || keysIn.empty()) {
			std::cout << "Invalid JOIN format, #channel key or [#channel1,#channel2] [key1,key2]" << std::endl;
			cli->addBuffer("Invalid JOIN format, #channel key or [#channel1,#channel2] [key1,key2]\r\n");
			return;
		}

		channels = splitString(channIn, ',');
		keys = splitString(keysIn, ',');

		if (channels.size() != keys.size()) {
			std::cout << "Number of channels does not match number of keys" << std::endl;
			return;
		}

		for (size_t i = 0; i < channels.size(); i++) 
		{
			if (channels[i].find("#") != 0) 
			{
				std::cout << "Invalid channel format. Channels must start with '#'." << std::endl;
				return;
			}

			channelKeyPairs.push_back(std::make_pair(channels[i], keys[i]));
			const std::string& channelName = channelKeyPairs[i].first;
			const std::string& channelKey = channelKeyPairs[i].second;

			if (validateChannelPassword(cli, channelName, channelKey, UserChannel)) 
			{
				std::string joinMessage = ":" + cli->getNickName() + " JOIN " + channelName + "\r\n";
				send(cli->getFd(), joinMessage.c_str(), joinMessage.size(), 0);
				// Notificar a otros clientes en el canal
				std::string notification = ":" + cli->getNickName() + " has joined " + channelName + "\r\n";
				infoAllServerClients(notification);
			}
			else
				return;
		}
	}
}


void Server::_authenticatePassword(Client *cli, std::vector<std::string> params)
{
	if (cli->getStatus() != PASS)
	{
		disconnectClient( cli, "462 * :" + params[0] + " may not reregister (need PASS state)\r\n", false );
	}
	else if (params.size() != 1)
	{
		cli->addBuffer("461 " + cli->getNickName() + " PASS :Incorrect parameters\r\n");
	}
	else if (params[0] != _password)
	{
		cli->addBuffer("464 * :Password " + params[0] + " incorrect " + _password + "\r\n");
	}
	else
	{
		std::cout << "Correct Password: " + params[0] + "\r\n";
		cli->nextStatus();
	}
}

void Server::_handleWhoIs(Client *cli, std::vector<std::string> params)
{
    // Check if a nickname was provided
    if (params.empty()) {
        cli->addBuffer(ERR_NONICK432);  // Error: No nickname provided
        return;
    }

    std::string targetNick = params[0];
    Client* targetClient = getClientNickName(targetNick);

    // Check if the client with the nickname exists
    if (!targetClient) {
        std::string error = "401 " + cli->getNickName() + " " + targetNick + " :No such nick/channel\r\n";
        cli->addBuffer(error);
        return;
    }

    // Send WHOIS details to the requesting client
    std::string whoisInfo = RES_WHOIS311;
    cli->addBuffer(whoisInfo);

    // Final message indicating the end of WHOIS information
    std::string endWhois = END_WHOIS318;
    cli->addBuffer(endWhois);
}

void	Server::_handleQuit(Client *cli, std::vector<std::string> params)
{
	disconnectClient(cli, params[0] , 0);
}

bool Server::validateChannelPassword(Client *cli, const std::string& channelName, const std::string& password, bool UserChannel)
{
    for (size_t i = 0; i < _channels.size(); i++) {
        if (_channels[i].getName() == channelName) 
		{
            // If the channel exists, validate the password
            if (_channels[i].getPass() == password) {
                std::cout << "Password is valid for channel: " << channelName << std::endl;
                _channels[i].addClient(cli);
                return true;
        	} 
			else 
			{
            std::cout << "Invalid password for channel: " << channelName << std::endl;
            return false;
			}
        }
    }
    std::string channelAdded = "Channel " + channelName + " added!\r\n";
    send(cli->getFd(), channelAdded.c_str(), channelAdded.size(), 0);
    addChannel(cli, channelName, password);
	if (UserChannel)
	{
		_channels[0].addClient(cli);
	}
    return true;
}

void Server::disconnectClient(Client *client, std::string msg, bool sendmsg)
{
	if (sendmsg)
	{
		std::string disconnect_msg = ":" + client->getNickName() + "!~" + client->getUserName() + " QUIT :" + msg + " \r\n";
		infoAllServerClients(disconnect_msg);
	}
    // Iterar sobre los canales para los que el cliente está registrado
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].isClient(client))
        {
			_channels[i].removeClient(client, client->getNickName());
			// Si el canal queda vacío, lo eliminamos
			if (_channels[i].getClients().size() == 0)
			{
				_channels.erase(_channels.begin() + i);
				i--;
			}
			else
            {
                // Si no hay administradores en el canal, asignar un nuevo administrador
                if (_channels[i].someAdmin() == 0)
                {
                    // Obtener el primer cliente del canal
                    std::vector<std::string> AllClients = _channels[i].getClients();
                    Client* newAdmin = getClientNickName(AllClients[0]);

                    // Asignar el nuevo administrador
                    _channels[i].setAdmin(newAdmin->getNickName());

                    // Notificar al nuevo administrador
                    std::string newAdminText = _channels[i].getName() + " No admins left connected/you are the new Admin \r\n";
                    newAdmin->addBuffer(newAdminText);
                }
            }
        }
    }
    clearClients(client->getFd());
}

void Server::handleConnection(Client *client)
{
	std::string serverName = "MyIRC";
	std::string version = "1.0";
    // Enviar mensaje de bienvenida a Irssi
	std::string welcomeMessage = ":" + serverName + " 001 " + client->getNickName() + " :Welcome to the " + serverName + " IRC Network, " + client->getNickName() + "\r\n";
	client->addBuffer(welcomeMessage);

	// Enviar información sobre el servidor
	std::string versionMessage = ":" + serverName + " 002 " + client->getNickName() + " :Your host is " + serverName + ", running version " + version + "\r\n";
    client->addBuffer(versionMessage);
}
