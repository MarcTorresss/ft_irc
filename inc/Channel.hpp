#ifndef CHANNEL_HPP_
# define CHANNEL_HPP_

# include "ircserv.hpp"
#include <iostream>
#include <string>
#include <sstream>

class Client;

class Channel
{
private:
	std::string	_channelName;
	std::string	_channelPassword;

    std::vector< std::string > _clients;
	std::vector< std::string > _admins;
    std::vector< std::string > _invites;

	int		_LimitUsersChannel;
	bool	_topic;

public:
	Channel( Client *client );

	bool	isClients( Client *client );
	bool	isAdmins( Client *client );
	bool	isInvites( Client *client );

	void	addClients( Client *client );
	void	addAdmins( Client *client );
	void	addInvites( Client *client );

	void	removeInvite( Client *client );
	void	removeTopic( Client *client );
	void	removeChannelPassword( Client *client );
	void	removeUsersLimitChannel( Client *client );

	~Channel();
};

#endif