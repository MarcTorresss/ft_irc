#include "Channel.hpp"

Channel::Channel( Client *client,  std::string name, std::string pass)
{
	_userLimit = 5;
	_channelName = name;
	_channelPassword = pass;
	_admins.push_back(client->getNickName());
	_clients.push_back(client->getNickName());
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
		throw std::runtime_error("Channel is full!");
	/*if (_inviteOnly && !isInvite(client))
		throw std::runtime_error("");*/
	_clients.push_back(client->getNickName());
}

void	Channel::addAdmin( Client *client, std::string target )
{
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	// if (!isClient(target))
		// throw std::runtime_error("That user is not connected to the channel");
	_admins.push_back(target);
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
		throw std::runtime_error("Not admin");
	unsigned long i = 0;
	for (i = 0; i < _admins.size(); ++i){
		if (_admins[i] == target)
			break;
	}
	std::vector< std::string>::iterator it = ( _admins.begin() + i );
	_admins.erase(it);
}

void	Channel::removeClient(Client *client, std::string target){
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	if (!isClient(client))
		throw std::runtime_error("That user is not connected to the channel");
	unsigned long i = 0;
	for (i = 0; i < _clients.size(); ++i){
		if (_clients[i] == target)
			break;
	}
	std::vector< std::string>::iterator it = ( _clients.begin() + i );
	_clients.erase(it);
}

void	Channel::addInvite( Client *client, std::string target )
{
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	_invites.push_back(target);
}

bool	Channel::isClient( Client *client )
{
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
	std::vector< std::string >::iterator it;
    for (it = _admins.begin(); it != _admins.end(); ++it)
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
		throw std::runtime_error("Not admin");
	_channelTopic = topic;
}

void Channel::setTopicAdmin(Client *client){
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	_adminTopic = !_adminTopic;
}

void Channel::setInviteOnly(Client *client){
	/*if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	_inviteOnly = !_inviteOnly;*/
	(void) client;
}

void	Channel::setPassword( Client *client, std::string pswd )
{
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	_channelPassword = pswd;
}

void	Channel::setUserLimit( Client *client, std::string limit )
{
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	if (limit.length() > 6)
		throw std::out_of_range("User limit not allowed (max. 999999)");
	for (int i = 0; limit[i] != 0; ++i){
		if (!isdigit(limit[i]))
			throw std::out_of_range("User limit must be a number (max. 999999)");
	}
	if (limit.length() == 0)
		_userLimit = -1;
	int lim = atoi(limit.c_str());
	if (lim < 0)
		throw std::out_of_range("User limit must be a positive int (max. 999999)");
	_userLimit = lim;
}

std::vector< std::string >	Channel::getClients()
{
	return _clients;
}

void Channel::setName(std::string name){_channelName = name;}

Channel::~Channel()
{

}