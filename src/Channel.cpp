#include "Channel.hpp"
Channel::Channel()
{
	std::cout << "DEF CONSTRUCTOR" << std::endl;
	_adminTopic = true;
}
Channel::Channel( Client *client,  std::string name, std::string pass) : _admins()
{
	_userLimit = 5;
	_channelName = name;
	_channelPassword = pass;
	_admins.push_back(client->getNickName());
	_clients.push_back(client->getNickName());
	_adminTopic = true;

}

std::string Channel::getName() const
{
	return _channelName;
}
std::string Channel::getPass() const
{
	return _channelPassword;
}

void	Channel::addClient( Client *client )
{
	if (_clients.size() >= _userLimit)
	{
		client->addBuffer("Channel is full!");
		return ;
	}
	/*if (_inviteOnly && !isInvite(client))
		throw std::runtime_error("");*/
	_clients.push_back(client->getNickName());
}

void	Channel::addAdmin( Client *client, Client *newadmin )
{
	if (!isAdmin( client ))
	{
		client->addBuffer("Not admin");
		return ;
	}
	_admins.push_back(newadmin->getNickName());
}

void	Channel::setAdmin( std::string NickName )
{
	_admins.push_back(NickName);
}

int		Channel::someAdmin( void )
{
	return _admins.size();
}

void	Channel::removeAdmin( Client *client, std::string target )
{
	if (!isAdmin( client ))
	{
		client->addBuffer("Not admin");
		return ;
	}
	unsigned long i = 0;
	for (i = 0; i < _admins.size(); ++i){
		if (_admins[i] == target)
			break;
	}
	std::vector< std::string>::iterator it = ( _admins.begin() + i );
	_admins.erase(it);
}

bool Channel::removeClient(Client *client, std::string target)
{
    if (!isAdmin(client))
	{
		client->addBuffer("Not admin");
		return false;
	}
    if (!isClient(client))
	{
		client->addBuffer("That user is not connected to the channel");
		return false;
	}
    unsigned long i = 0;
    for (i = 0; i < _clients.size(); ++i)
	{
        if (_clients[i] == target)
            break;
    }
    if (i < _clients.size())
	{
        std::vector<std::string>::iterator it = (_clients.begin() + i);
        _clients.erase(it);
        return true;
    }
    return false;
}

void	Channel::addInvite( std::string target )
{
	_invites.push_back(target);
}

bool	Channel::isClient( Client *client )
{
	if (client == NULL) {
        std::cerr << "Error: Client is NULL" << std::endl;
        return false;
    }
    if (_clients.empty()) {
        std::cerr << "Warning: No admins in the channel" << std::endl;
        return false;
    }
	std::vector< std::string >::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
	{
    	if (*it == client->getNickName())
			return true;
	}
	return false;
}

bool	Channel::isAdmin( Client *client )
{
	if (client == NULL) {
        std::cerr << "Error: Client is NULL" << std::endl;
        return false;
    }
    if (_admins.empty()) {
        std::cerr << "Warning: No admins in the channel" << std::endl;
        return false;
    }
	std::vector< std::string >::iterator it;
   	for (it = _admins.begin(); it != _admins.end(); it++)
	{
    	if (*it == client->getNickName())
			return true;
	}
	return false;
}

bool	Channel::isInvite( Client *client )
{
	std::vector< std::string >::iterator it;
    for (it = _invites.begin(); it != _invites.end(); ++it)
	{
    	if (*it == client->getNickName())
			return true;
	}
	return false;
}

void	Channel::removeInvite( Client *client, std::string target)
{
	std::vector< std::string >::iterator it;
    for (it = _invites.begin(); it != _invites.end(); ++it)
	{
    	if (*it == target){
			if (isAdmin(client))
				_invites.erase(it);
		}
	}
}

void	Channel::setTopic( Client *client, std::string topic )
{
	if (_adminTopic && !isAdmin( client ))
	{
		client->addBuffer("Not admin");
		return ;
	}
	_channelTopic = topic;
}

void Channel::setTopicAdmin(Client *client){
	if (!isAdmin( client ))
	{
		client->addBuffer("Not admin");
		return ;
	}
	_adminTopic = !_adminTopic;
}

void Channel::setInviteOnly(Client *client){
	if (!isAdmin( client ))
	{
		client->addBuffer("Not admin");
		return ;
	}
	_inviteOnly = !_inviteOnly;
}

void	Channel::setPassword( Client *client, std::string pswd )
{
	if (!isAdmin( client ))
	{
		client->addBuffer("Not admin");
		return ;
	}
	_channelPassword = pswd;
}

void	Channel::setUserLimit( Client *client, std::string limit )
{
	if (!isAdmin( client ))
	{
		client->addBuffer("Not admin");
		return ;
	}
	if (limit.length() > 6)
	{
		client->addBuffer("User limit not allowed (max. 999999)");
		return ;
	}
	for (int i = 0; limit[i] != 0; ++i){
		if (!isdigit(limit[i]))
		{
			client->addBuffer("User limit must be a number (max. 999999)");
			return ;
		}
	}
	if (limit.length() == 0)
		_userLimit = -1;
	int lim = atoi(limit.c_str());
	if (lim <= 0)
	{
		client->addBuffer("User limit must be a positive int (max. 999999)");
		return ;
	}
	if (_clients.size() > (size_t)lim)
	{
		client->addBuffer("User limit cant be smaller than user count");
		return ;
	}
	_userLimit = lim;
}

std::vector< std::string >	Channel::getClients()
{
	return _clients;
}

void Channel::setName(std::string name){_channelName = name;}

std::string	Channel::getTopic()
{
	return _channelTopic;
}

bool	Channel::istopiclock( void )
{
	return _adminTopic;
}

const std::string& Channel::getPassword() const {
    return _channelPassword;
}

bool Channel::isAdminTopicEnabled() const {
    return _adminTopic;
}

bool Channel::isInviteOnly() const {
    return _inviteOnly;
}

int Channel::getUserLimit() const {
    return _userLimit;
}

Channel::~Channel()
{

}

void Channel::showModes(){
	std::cout << "--CHANNEL MODES-> ";
	std::cout << "i(invite)=" << _inviteOnly<<"/ ";
	std::cout << "t(topic)=" << _adminTopic<<"/ ";
	std::cout << "k(password)=" << _channelPassword<<"/ ";
	std::cout << "l(user lim)=" << _userLimit<<"/ ";

	std::cout << "o(admins)=" << _channelPassword;
	std::vector< std::string >::iterator it;
   	for (it = _admins.begin(); it != _admins.end(); it++)
		std::cout << *it << "-";
	std::cout <<std::endl;
}