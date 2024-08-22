#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
//-------------------------------------------------------//
#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"
//-------------------------------------------------------//
class Client
{
private:
	int Fd;
	std::string IPadd; //-> client ip address
public:
	Client(){};
	int GetFd(){return Fd;}

	void SetFd(int fd){Fd = fd;}
	void setIpAdd(std::string ipadd){IPadd = ipadd;}
};

class Server
{
private:
	int Port;
	int SerSocketFd;
	static bool Signal;
	std::vector<Client> clients;
	std::vector<struct pollfd> fds; //-> vector of pollfd
public:
	Server(){SerSocketFd = -1;}

	void ServerInit();
	void SerSocket();
	void AcceptNewClient();
	void ReceiveNewData(int fd); //-> receive new data from a registered client

	static void SignalHandler(int signum);
 
	void CloseFds();
	void ClearClients(int fd);
};

//-------------------------------------------------------//
void Server::ClearClients(int fd){
	for(size_t i = 0; i < fds.size(); i++){ //-> remove the client from the pollfd
		if (fds[i].fd == fd)
			{fds.erase(fds.begin() + i); break;}
	}
	for(size_t i = 0; i < clients.size(); i++){ //-> remove the client from the vector of clients
		if (clients[i].GetFd() == fd)
			{clients.erase(clients.begin() + i); break;}
	}

}