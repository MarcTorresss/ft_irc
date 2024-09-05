#ifndef CHANNEL_HPP_
# define CHANNEL_HPP_

# include "ircserv.hpp"

class Client;

class Channel
{
private:
	std::string	_channelName;
	std::string	_channelPassword;
	// std::string	_channelTopic;

    std::vector< std::string > _clients;
	std::vector< std::string > _admins;
    std::vector< std::string > _invites;

	int		_LimitUsersChannel;
	bool	_topic;

public:
	Channel();

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

	std::string getName();
	void setName(std::string name);
	std::string getPassword();
	void setPassword(std::string password);
	// std::string getTopic();
	// void setTopic(std::string topic);

	~Channel();
};

#endif