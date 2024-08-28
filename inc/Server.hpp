#ifndef SERVER_HPP_
# define SERVER_HPP_

#include "ircserv.hpp"
class Client;

class Server
{
private:
	int _serSocketFd;
	int _port;
	static bool _signal;
	std::vector<Client> _clients;
	std::vector<struct pollfd> _fds; //-> vector of pollfd
public:
	Server();
	Server(int port);
	~Server();

	void serverInit();
	void serSocket();
	void acceptNewClient();
	void receiveNewData(int fd); //-> receive new data from a registered client

	static void signalHandler(int signum);

	void closeFds();
	void clearClients(int fd);
};

#endif