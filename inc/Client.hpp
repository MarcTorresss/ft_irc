#ifndef CLIENT_HPP_
# define CLIENT_HPP_

#include "ircserv.hpp"

class Client
{
private:
	int _fd;
	std::string _ipAdd; //-> client ip address
public:
	Client();
	Client(int fd, std::string ipAdd);
	~Client();

	int getFd();
	void setFd(int fd);
	void setIpAdd(std::string ipaAdd);
};

#endif