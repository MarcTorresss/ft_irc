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
	static bool _signal;
	std::vector<Client> _clients;
	std::vector<Channel> _channels;
	std::vector<struct pollfd> _fds;

    Client* getClient(int fd);

	void	_authenticatePassword(Client *cli, std::string& params);
    void	_setNickname(Client *cli, std::string& params);
    void	_setUser(Client *cli, std::string& params);
    void	_sendPing(Client *cli, std::string& params);
    void	_handleCapabilities(Client *cli, std::string& params);
    void	_handleQuit(Client *cli, std::string& params);
    void	_handleJoin(Client *cli, std::string& params);
    void	_handlePrivmsg(Client *cli, std::string& params);
    void	_handleKick(Client *cli, std::string& params);
    void	_handleInvite(Client *cli, std::string& params);
    void	_handleTopic(Client *cli, std::string& params);
    void	_handleMode(Client *cli, std::string& params);

public:
	Server();
	Server(int port);
	~Server();

	void	serverInit();
	void	serSocket();
	void	acceptNewClient();
	void	receiveNewData(int fd);
	void	check_comand( char *buff, Client *cli );


	static void signalHandler(int signum);

	void closeFds();
	void clearClients(int fd);
};

#endif