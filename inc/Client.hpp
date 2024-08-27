#ifndef CLIENT_HPP_
# define CLIENT_HPP_

#include "ircserv.hpp"

class Client
{
private:
	int Fd;
	std::string IPadd; //-> client ip address
public:
	Client();
	Client(int fd, std::string ipadd);
	~Client();

	int GetFd();
	void SetFd(int fd);
	void setIpAdd(std::string ipadd);
};

#endif