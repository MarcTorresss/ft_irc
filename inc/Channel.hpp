#ifndef CHANNEL_HPP_
# define CHANNEL_HPP_

# include "ircserv.hpp"

class Client;

class Channel
{
private:
	std::string	_channelName;
	std::string	_channelPassword;
    std::vector< std::string > _clients;
	std::vector< std::string > _admins;
    std::vector< std::string > _invites;

public:
	Channel();
	bool	isClients( Client *client );
	bool	isAdmins( Client *client );
	bool	isInvites( Client *client );
	void	addClients( Client *client );
	void	addAdmins( Client *client );
	void	addInvites( Client *client );
	~Channel();
};

#endif