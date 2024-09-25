#include "ircserv.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void	Server::_setNickname(Client *cli, std::string& params){
	if (params == ""){
		std::cout << "client " << cli->getFd() << " NICK is: " << cli->getNickName() << "-" << std::endl;
	}
	if (cli->getStatus() >= PASS){
		std::cout << "NICK changed" << std::endl;
		cli->setNickName(params);
	}
	else{
		std::cout << ERR << "cannot change nick if password is not set" << std::endl;
		//send message to client
	}
}

void	Server::_setUser(Client *cli, std::string& params){
	if (params == ""){
		std::cout << "client <" << cli->getFd() << "> USR is: " << cli->getUserName() << "-" << std::endl;
	}
	if (cli->getStatus() >= NICK){
		std::cout << "USR changed" << std::endl;
		cli->setUserName(params);
	}
	else{
		std::cout << ERR << "cannot change username if nickname is not set" << std::endl;
		//send err message to client
	}
}

void	Server::joinChannel(Client *cli, std::string& params){
	(void) cli;
	(void) params;
	std::cout << "client <" << cli->getFd() << "> wants to join channel " << params << std::endl;
	for (int i = 0; _channels.size(); ++i){
		if (_channels[i].getName() == params)
			_channels[i].addClient(cli);//join channel + check channel perms (password, invite only, has been invited?)
	}
	//else create channel
}

void	Server::_handlePrivmsg(Client *cli, std::string& params){
	(void) cli;
	(void) params;
	//create a private(invite only) channel between 2 clients
}

void	Server::_handleKick(Client *cli, std::string& params){
	int channelIdx = getChannelIndex();
	_channels[channelIdx].removeClient(cli, params);
}

void	Server::_handleInvite(Client *cli, std::string& params){
	int channelIdx = getChannelIndex();
	//does the client need to be connected to server to be invited??
	_channels[channelIdx].addInvite(cli, params);
	//2. notify user?
}

void	Server::_handleTopic(Client *cli, std::string& params){
	int channelIdx = getChannelIndex();
	_channels[channelIdx].setTopic(cli, params);
}

void	Server::_handleMode(Client *cli, std::string& params){
	if (params == ""){
		std::cout << "the current channel modes are [" << "]" <<std::endl; //print los channel modes
	}
	std::string modes[] = {"i","t","k","o","l"};
	int i = 0;
	for (i = 0; i < 5; ++i){
		if (modes[i] == params)
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
				_channels[0].setPassword(cli,params);
				break;
			case 3: //o
				_channels[0].addAdmin(cli,params);
				break;
			case 4: //l
				_channels[0].setUserLimit(cli,params);
				break;
			default:
				std::cout << ERR << "Channel MODE not existent [i, t, k, o, l]" <<std::endl;
				break;
		}
	}catch(std::exception &e){
		std::cout << ERR << e.what() <<std::endl;
	}
}

void	Server::_handlePing(Client *cli, std::string& params)
{
    (void) params;
    cli->addBuffer("Ping Marc Pong Albert and javi from munich\r\n");
}

void Server::addChannel(const std::string& channelName, const std::string& password) {
    (void) channelName;
    (void) password;
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

void Server::_handleJoin(Client *cli, std::string& params)
{
    //param format [#channel1,#channel2 key1,key2]
    std::vector<std::pair<std::string, std::string> > channelKeyPairs;    std::vector<std::string> channels, keys;
    std::istringstream iss(params);
    std::string channIn, keysIn;

    std::getline(iss, channIn, ' ');
    std::getline(iss, keysIn);

    std::cout << "Channels: " << channIn << std::endl;
    std::cout << "Keys: "<< keysIn << std::endl;

    if (channIn.empty() || keysIn.empty())
        {std::cout << "Invalid JOIN format, [#channel1,#channel2] [key1,key2]" << std::endl; return;}

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
    addChannel(channelName, password);
    return true;
}

void Server::disconnectClient(Client *client, std::string msg)
{
    std::string disconnect_msg = ":" + client->getNickName() + "!~" + client->getUserName() + " QUIT :" + msg + " \r\n";
    infoAllServerClients(disconnect_msg);
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