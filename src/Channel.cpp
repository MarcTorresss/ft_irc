#include "Channel.hpp"

Channel::Channel( Client *client )
{
	_admins.push_back(client->getNickName());
	_clients.push_back(client->getNickName());
}
std::string Channel::getName()
{
	return _channelName;
}
std::string Channel::getPass()
{
	return _channelPassword;
}
void	Channel::addClients( Client *client )
{
	_clients.push_back(client->getNickName());
}

void	Channel::addAdmins( Client *client )
{
	_admins.push_back(client->getNickName());
}

void	Channel::addInvites( Client *client )
{
	_invites.push_back(client->getNickName());
}

bool	Channel::isClients( Client *client )
{
	std::vector< std::string >::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
	{
    	if (*it == client->getNickName())
			return true;
	}
	return false;
}

bool	Channel::isAdmins( Client *client )
{
	std::vector< std::string >::iterator it;
    for (it = _admins.begin(); it != _admins.end(); ++it)
	{
    	if (*it == client->getNickName())
			return true;
	}
	return false;
}

bool	Channel::isInvites( Client *client )
{
	std::vector< std::string >::iterator it;
    for (it = _invites.begin(); it != _invites.end(); ++it)
	{
    	if (*it == client->getNickName())
			return true;
	}
	return false;
}

void	Channel::removeInvite( Client *client )
{
	std::vector< std::string >::iterator it;
    for (it = _invites.begin(); it != _invites.end(); ++it)
	{
    	if (*it == client->getNickName())
			_invites.erase(it);
	}
}

void	Channel::removeTopic( Client *client )
{
	if (isAdmins( client ))
		_topic = false;
}

void	Channel::removeChannelPassword( Client *client )
{
	if (isAdmins( client ))
		_channelPassword = "";
}

void	Channel::removeUsersLimitChannel( Client *client )
{
	if (isAdmins( client ))
		_LimitUsersChannel = 0;
}

Channel::~Channel()
{

}