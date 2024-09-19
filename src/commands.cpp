#include "ircserv.hpp"
#include "utils.cpp"

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

void Server::addChannel(const std::string& channelName, const std::string& password) {
    (void) channelName;
    (void) password;
}

void Server::_handleJoin(Client *cli, std::string& params)
{
    //param format [#channel1,#channel2 key1,key2]
    std::vector<std::pair<std::string, std::string> > channelKeyPairs;    std::vector<std::string> channels, keys;
    std::istringstream iss(params);
    std::string command, channIn, keysIn;
    iss >> command;

    std::getline(iss, channIn, ' ');
    std::getline(iss, keysIn);

    if (channIn.empty() || keysIn.empty())
        throw std::invalid_argument("Invalid JOIN format, [#channel1,#channel2] [key1,key2]");
    
    channels = splitString(channIn, ',');
    keys = splitString(keysIn, ',');

    if (channels.size() != keys.size())
        throw std::invalid_argument("Number of channels does not match number of keys");
    
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].find("#") != 0)
            throw std::invalid_argument("Invalid channel format. Channels must start with '#'.");
        channelKeyPairs.push_back(std::make_pair(channels[i], keys[i]));
        const std::string& channelName = channelKeyPairs[i].first;
        const std::string& channelKey = channelKeyPairs[i].second;
        validateChannelPassword(cli, channelName, channelKey);
    }
}