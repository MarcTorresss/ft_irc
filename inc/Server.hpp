#ifndef SERVER_HPP_
# define SERVER_HPP_

#include "ircserv.hpp"
#include "Channel.hpp"
#include <iostream>
#include <string>
#include <sstream>

# define BUFF_SIZE 1024

class Channel;
class Client;

class Server
{
private:
	int _serSocketFd;
	int _port;
	std::vector<Client> _clients;
	std::vector<Channel> _channels;
	std::vector<struct pollfd> _fds;
	std::string _password; //pass del server o del cliente que te pasa

    Client* getClient(int fd);

	void	_authenticatePassword(Client *cli, std::vector<std::string> params);
    void	_setNickname(Client *cli, std::vector<std::string> params);
    void	_setUser(Client *cli, std::vector<std::string> params);
    void	_sendPing(Client *cli, std::vector<std::string> params);
    void	_handleCapabilities(Client *cli, std::vector<std::string> params);
    void	_handleQuit(Client *cli, std::vector<std::string> params);
	void	_handleJoin(Client *cli, std::vector<std::string> params, bool UserChannel);
    void	_handlePrivmsg(Client *cli, std::vector<std::string> params);
    void	_handleKick(Client *cli, std::vector<std::string> params);
    void	_handleInvite(Client *cli, std::vector<std::string> params);
    void	_handleTopic(Client *cli, std::vector<std::string> params);
    void	_handleMode(Client *cli, std::vector<std::string> params);
	void	_handlePing(Client *cli, std::vector<std::string> params);
	void	_handleWhoIs(Client *cli, std::vector<std::string> params);

public:
	Server();
	Server( int port, std::string pass );
	~Server();
	// static void signalHandler(int signum);
	void		createSocket();
	void 		loop();
	void		acceptNewClient();
	void		receiveNewData(int fd);
	void		check_comand(Client *cli );

	void 		closeFds();
	void 		clearClients(int fd);
	int			getChannelIndex( std::string channelName );
	void 		addChannel(Client *cli, const std::string& channelName, const std::string& password);
	bool		validateChannelPassword(Client *cli, const std::string& channelName, const std::string& password, bool UserChannel);
  	void		disconnectClient(Client *client, std::string msg, bool sendmsg);
	void		infoAllServerClients( std::string msg );
	Client		*getClientNickName( std::string NickName );
	bool		getIsNickNameInUse( std::string NickName );
	void		handleConnection(Client *client);

	Channel		*findChannel( std::string nameChannel );
	void		sendMsgToChannel( int fd, std::string msg, Channel *channel );

	//Debugging
	const		std::vector<Channel>& getChannels() const;
	void		getChannelsList() const;
};

#endif