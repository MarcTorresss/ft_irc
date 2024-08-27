#ifndef SERVER_HPP_
# define SERVER_HPP_

#include "ircserv.hpp"
class Client;

class Server
{
private:
	int Port;
	int SerSocketFd;
	static bool Signal;
	std::vector<Client> clients;
	std::vector<struct pollfd> fds; //-> vector of pollfd
public:
	Server();
	Server(int port);
	~Server();

	void ServerInit();
	void SerSocket();
	void AcceptNewClient();
	void ReceiveNewData(int fd); //-> receive new data from a registered client

	static void SignalHandler(int signum);
 
	void CloseFds();
	void ClearClients(int fd);
};

#endif