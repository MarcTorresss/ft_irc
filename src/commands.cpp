#include "ircserv.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void	Server::_setNickname(Client *cli, std::vector<std::string> params)
{
	if (cli->getStatus() == DONE)
	{
		cli->setNickName(params[0]);
		infoAllServerClients( std::string( "The new NickName is " + params[0] + "\r\n"));
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
	if (cli->getStatus() != USER)
	{
		cli->addBuffer("462 * : USER not reregistred ( need PASS and NICK )\r\n");
	}
	else
	{
		cli->setUserName(params[1]);
		std::cout << "User saved: " << params[1] << std::endl;
		handleConnection(cli);
		cli->nextStatus();
		// se ara un join a un canal con el nombre del cliente de forma automatica
	}
}

void	Server::_handlePrivmsg(Client *cli, std::vector<std::string> params)
{
	(void) params;
    (void) cli;
	/*if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
 	// destinatario y mensaje
    if (params.size() < 2)
    {
        cli->addBuffer(ERR_PARAM461);
        return;
    }

    std::string target = params[0]; // El destinatario (cliente o canal)
    std::string message = params[1]; // El mensaje (puedes unir los params si hay espacios)

    // Comprobar si el destinatario es un cliente o un canal
    if (isChannel(target))
    {
        // Enviar mensaje a todos los usuarios del canal
        Channel *channel = findChannel(target);
        if (channel)
        {
            channel->broadcastMessage(cli, message);
        }
        else
        {
            cli->addBuffer("401 " + target + " :No such channel\r\n");
        }
    }
    else
    {
        // Enviar mensaje a un cliente específico
        Client *recipient = findClient(target);
        if (recipient)
        {
            recipient->addBuffer(":" + cli->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n");
        }
        else
        {
            cli->addBuffer("401 " + target + " :No such nick\r\n");
        }
    }*/
}

void	Server::_handleKick(Client *cli, std::vector<std::string> params){
	if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{
		int channelIdx = getChannelIndex();
		_channels[channelIdx].removeClient(cli, params[0]);
	}
}

void	Server::_handleInvite(Client *cli, std::vector<std::string> params){
	if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{
		int channelIdx = getChannelIndex();
		//does the client need to be connected to server to be invited??
		_channels[channelIdx].addInvite(cli, params[0]);
		//2. notify user?
	}
}

void	Server::_handleTopic(Client *cli, std::vector<std::string> params){
	if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{	
		int channelIdx = getChannelIndex();
		_channels[channelIdx].setTopic(cli, params[0]);
	}
}

void	Server::_handleMode(Client *cli, std::vector<std::string> params){
	/*if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{*/
		for (size_t i = 0; i < params.size(); ++i) {
			std::cout << "Param " << i << ": " << params[i] << std::endl;
		}
		std::string type = params[params.size() - 1];
		std::cout << type << std::endl;
		if (params.empty()){
			std::cout << "the current channel modes are [" << "]" <<std::endl; //print los channel modes
		}
		std::string modes[] = {"+i","t","k","o","l"};
		int i = 0;
		for (i = 0; i < 5; ++i){
			// std::cout << params[params.size() - 1] << std::endl;
			// for (size_t i = 0; i < params.size(); ++i) {
			// 	std::cout << "Param " << i << ": " << params[i] << std::endl;
			// }
			// if (modes[i] == params[params.size() - 1])
			// 	break;
			if (modes[i] == type)
				break;
		}
		try{
			switch (i) //DONE
			{
				case 0: //i
					_channels[0].setInviteOnly(cli);
					break;
				case 1: //t
					_channels[0].setTopicAdmin(cli);
					break;
				case 2: //k
					_channels[0].setPassword(cli,params[0]);
					break;
				case 3: //o
					_channels[0].addAdmin(cli,params[0]);
					break;
				case 4: //l
					_channels[0].setUserLimit(cli,params[0]);
					break;
				default:
					std::cout << ERR << "Channel MODE not existent [i, t, k, o, l]" <<std::endl;
					break;
			}
		}catch(std::exception &e){
			std::cout << ERR << e.what() <<std::endl;
		}
	//}
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
		std::cout << "test" << std::endl;
    	cli->addBuffer("Ping Marc Pong Albert and javi from munich\r\n");
	}
}

void Server::addChannel(Client *cli, const std::string& channelName, const std::string& password) {
    Channel newChannel(cli, channelName, password);
    _channels.push_back(newChannel);
    getChannelsList();
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

void Server::_handleJoin(Client *cli, std::vector<std::string> params)
{
	if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" << std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{
		//param format [#channel1,#channel2 key1,key2]
		std::vector<std::pair<std::string, std::string> > channelKeyPairs;    std::vector<std::string> channels, keys;
		std::istringstream namechanel(params[0]);
		std::istringstream passchanel(params[1]);
		std::string channIn, keysIn;

		std::cout << params[0] << std::endl;
		std::getline(namechanel, channIn, ' ');
		std::cout << "Channels: " << channIn << std::endl;
		std::getline(passchanel, keysIn);
		std::cout << "Keys: "<< keysIn << std::endl;


		if (channIn.empty() || keysIn.empty())
		{
			std::cout << "Invalid JOIN format, #channel key or [#channel1,#channel2] [key1,key2]" << std::endl;
			cli->addBuffer("Invalid JOIN format, #channel key or [#channel1,#channel2] [key1,key2]\r\n");
			return;
		}
		channels = splitString(channIn, ',');
		keys = splitString(keysIn, ',');

		if (channels.size() != keys.size())
			{std::cout << "Number of channels does not match number of keys" << std::endl; return;}   
		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i].find("#") != 0)
				{std::cout << "Invalid channel format. Channels must start with '#'." << std::endl; return;}
			channelKeyPairs.push_back(std::make_pair(channels[i], keys[i]));
			const std::string& channelName = channelKeyPairs[i].first;
			const std::string& channelKey = channelKeyPairs[i].second;
			validateChannelPassword(cli, channelName, channelKey);
		}
		//parse channel and pass
		//if channel exists
			//check password
				//join client
		//else addChannel
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

void	Server::_handleWhoIs(Client *cli, std::vector<std::string> params)
{
	(void) cli;
	(void) params;
}

bool Server::validateChannelPassword(Client *cli, const std::string& channelName, const std::string& password) {
    
    for (size_t i = 0; i < _channels.size(); i++) {
        if (_channels[i].getName() == channelName) {
            // If the channel exists, validate the password
            if (_channels[i].getPass() == password) {
                std::cout << "Password is valid for channel: " << channelName << std::endl;
                _channels[i].addClient(cli);
                return true;
            } else {
                std::cout << "Invalid password for channel: " << channelName << std::endl;
                return false;
            }
        }
    }
	std::string channelAdded = "Channel " + channelName + " added!\r\n";
    send(cli->getFd(), channelAdded.c_str(), channelAdded.size(), 0);
    addChannel(cli, channelName, password);
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
