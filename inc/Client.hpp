#ifndef CLIENT_HPP_
# define CLIENT_HPP_

# include "ircserv.hpp"

class Client
{
private:
	std::string	_userName;
	std::string	_nickName;
	int _fd;
	std::string			_ipAdd; //-> client ip address
	struct sockaddr_in	_clientAdd; // direccion i puerto del cliente
	std::string			_sendBuffer;
	std::string			_receiveBuffer;

public:
	Client();
	Client(int fd, std::string ipAdd);
	~Client();

	std::string getUserName();
    std::string getNickName();
	int getFd();
	std::string getIpAdd();
    struct sockaddr_in getClientAdd();
    std::string getSendBuffer();
    std::string getReceiveBuffer();
	void setFd( int fd );
	void setIpAdd( std::string ipAdd);
	void setUserName( std::string& userName );
    void setNickName( std::string& nickName );
    void setClientAdd( struct sockaddr_in& clientAdd );
    void setSendBuffer( std::string& sendBuffer );
    void setReceiveBuffer( std::string& receiveBuffer );
};

#endif