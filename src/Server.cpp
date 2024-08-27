#include "Server.hpp"

Server::Server(){
	SerSocketFd = -1;
	Port = 4444;
}

Server::Server(int port){
	SerSocketFd = -1;
	Port = port;
}

Server::~Server(){

}

void Server::ServerInit(){

}
void Server::SerSocket(){
	struct sockaddr_in add;
	// struct pollfd poll_a; //;)

	add.sin_family = AF_INET;
	add.sin_port = htons(Port);
	add.sin_addr.s_addr = INADDR_ANY;
}


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