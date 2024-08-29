#include "Server.hpp"

bool Server::_signal = false;

Server::Server(): _serSocketFd(-1), _port(4444){
}

Server::Server(int port): _serSocketFd(-1), _port(port){
}

Server::~Server(){

}

void Server::serverInit(){
}

void Server::serSocket(){
	struct sockaddr_in add;
	// struct pollfd poll_a; //;)

	add.sin_family = AF_INET;
	add.sin_port = htons(_port);
	add.sin_addr.s_addr = INADDR_ANY;
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