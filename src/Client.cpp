#include "Client.hpp"

Client::Client(): _fd(-1), _ipAdd(""){
}

Client::Client(int fd, std::string ipadd): _fd(fd), _ipAdd(ipadd){
}

Client::~Client(){
}

int Client::getFd(){
	return _fd;
}

void Client::setFd(int fd){
	_fd = fd;
}

void Client::setIpAdd(std::string ipadd){
	_ipAdd = ipadd;
}