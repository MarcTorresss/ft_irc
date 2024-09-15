#ifndef CHANNEL_HPP_
# define CHANNEL_HPP_

# include "ircserv.hpp"

class Client;

class Channel
{
private:
	std::string	_channelName;
	std::string	_channelPassword;
	std::string	_channelTopic;

	bool	_adminTopic;
	bool	_inviteOnly;
	int		_userLimit;
    std::vector< std::string > _clients;
	std::vector< std::string > _admins;
    std::vector< std::string > _invites;

public:
	Channel();
	~Channel();

	bool	isClient( Client *client );
	bool	isAdmin( Client *client );
	bool	isInvite( Client *client );

	void	addAdmin( Client *client, std::string target );
	void	addInvite( Client *client, std::string target );
	void	addClient( Client *client);
	void	removeAdmin( Client *client, std::string target );
	void	removeInvite( Client *client, std::string target );
	void	removeClient(Client *client, std::string target);

	std::string getName();
	std::string getTopic();
	std::string getPassword();

	void setUserLimit( Client *client, std::string limit );
	void setName(std::string name);
	void setPassword(Client *client, std::string pswd );
	void setInviteOnly(Client *client);
	void setTopic( Client *client, std::string topic);
	void setTopicAdmin(Client *client);
};

#endif