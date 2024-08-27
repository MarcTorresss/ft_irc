#include "Client.hpp"

Client::Client(){
	Fd = -1;
	IPadd = "";
}

Client::Client(int fd, std::string ipadd): Fd(fd), IPadd(ipadd){
}

Client::~Client(){
}

int Client::GetFd(){
	return Fd;
}

void Client::SetFd(int fd){
	Fd = fd;
}

void Client::setIpAdd(std::string ipadd){
	IPadd = ipadd;
}