#ifndef SERVER_HPP_
# define SERVER_HPP_

#include "ircserv.hpp"
#include <iostream>
#include <string>
#include <sstream>

# define BUFF_SIZE 1024

class Client;

class Server
{
private:
	int _serSocketFd;
	int _port;
	std::vector<Client> _clients;
	std::vector<struct pollfd> _fds;

    Client* getClient(int fd);

	void	_authenticatePassword(Client *cli, const std::string& params);
    void	_setNickname(Client *cli, const std::string& params);
    void	_setUser(Client *cli, const std::string& params);
    void	_sendPing(Client *cli, const std::string& params);
    void	_handleCapabilities(Client *cli, const std::string& params);
    void	_handleQuit(Client *cli, const std::string& params);
    void	_handleJoin(Client *cli, const std::string& params);
    void	_handlePrivmsg(Client *cli, const std::string& params);
    void	_handleKick(Client *cli, const std::string& params);
    void	_handleInvite(Client *cli, const std::string& params);
    void	_handleTopic(Client *cli, const std::string& params);
    void	_handleMode(Client *cli, const std::string& params);

public:
	Server();
	Server(int port);
	~Server();

	void	serverInit();
	void	serSocket();
	void	acceptNewClient();
	void	receiveNewData(int fd);
	void	check_comand( char *buff, Client *cli );

	void closeFds();
	void clearClients(int fd);
};

#endif