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
	std::string	_channelTopic;

	bool	_adminTopic;
	bool	_inviteOnly;
	unsigned long	_userLimit;
    std::vector< std::string > _clients;
	std::vector< std::string > _admins;
    std::vector< std::string > _invites;

public:
	Channel();
	Channel( Client *client, std::string name, std::string pass);

	~Channel();

	bool	isClient( Client *client );
	bool	isAdmin( Client *client );
	bool	isInvite( Client *client );

	void	addAdmin( Client *client, std::string target );
	void	addInvite( std::string target );
	void	addClient( Client *client);
	void	removeAdmin( Client *client, std::string target );
	void	removeInvite( Client *client, std::string target );
	bool	removeClient(Client *client, std::string target);

	std::string getName() const;
	std::string getTopic();
	std::string getPass() const;

	void	setUserLimit( Client *client, std::string limit );
	void	setName(std::string name);
	void	setPassword(Client *client, std::string pswd );
	void	setInviteOnly(Client *client);
	void	setTopic( Client *client, std::string topic);
	void	setTopicAdmin(Client *client);
	int		someAdmin( void );
	void	setAdmin( std::string NickName );
	bool	istopiclock( void );

	const std::string& getPassword() const;
    bool isAdminTopicEnabled() const;
    bool isInviteOnly() const;
    int getUserLimit() const;

	std::vector< std::string >	getClients();
};

#endif