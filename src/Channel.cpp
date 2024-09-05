#include "Channel.hpp"

Channel::Channel()
{


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


Channel::~Channel()
{

}