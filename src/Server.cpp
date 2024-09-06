#include "Server.hpp"

bool Server::_signal = false;

Server::Server(): _serSocketFd(-1), _port(4444){
	serverInit();
}

Server::Server(int port): _serSocketFd(-1), _port(port){
	serverInit();
}

Server::~Server(){

}

void Server::serverInit(){
	serSocket();

	struct pollfd serPoll;
	serPoll.fd = _serSocketFd; //el port d'escolta del serversocket
	serPoll.events = POLLIN; //pendent de input events
	_fds.push_back(serPoll); //afagir el pollfd al vector
}

void Server::signalHandler(int signum){
	(void)signum;
	Server::_signal = true;
}

void Server::serSocket(){
	struct sockaddr_in sockAddr;
	// struct pollfd poll_a; //;)

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(_port);
	sockAddr.sin_addr.s_addr = INADDR_ANY;
	std::memset(sockAddr.sin_zero, 0, sizeof(sockAddr.sin_zero));
}

void Server::acceptNewClient(){
	Client client;

	struct sockaddr_in cliAddr;
	socklen_t len = sizeof(cliAddr);
	//struct pollfd cliPoll;
	int cliSocket = accept(_serSocketFd, (struct sockaddr *)&(cliAddr), &len);
	if (cliSocket < 0) {
		std::cout << "unable to accept new client" << std::endl;
		return;
	}
}
void Server::clearClients(int fd){
	for(size_t i = 0; i < _fds.size(); i++){ //-> remove the client from the pollfd
		if (_fds[i].fd == fd)
			{_fds.erase(_fds.begin() + i); break;}
	}
	for(size_t i = 0; i < _clients.size(); i++){ //-> remove the client from the vector of clients
		if (_clients[i].getFd() == fd)
			{_clients.erase(_clients.begin() + i); break;}
	}

}