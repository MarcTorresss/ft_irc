#ifndef CLIENT_HPP_
# define CLIENT_HPP_

# include "ircserv.hpp"

class Client
{
private:
	std::string	_userName;
	std::string	_nickName;
	int _fd;
	int _status;
	std::string			_ipAdd; //-> client ip address
	struct sockaddr_in	_clientAdd; // direccion i puerto del cliente
	std::string			_buffer;

public:
	Client();
	Client(int fd, std::string ipAdd);
	~Client();

	std::string getUserName( void );
    std::string getNickName( void );
	int getFd( void );
	std::string getIpAdd( void );
    struct sockaddr_in getClientAdd( void );
    std::string getBuffer( void );
	void	setFd( int fd );
	void	setIpAdd( std::string ipAdd);
	void	setUserName( std::string& userName );
    void	setNickName( std::string& nickName );
    void 	setClientAdd( struct sockaddr_in& clientAdd );
	void	setBuffer( std::string buffer );
    int		sendInfo( void );
	int		receiveInfo( void );
	void	setStatus(int status);
	int		getStatus();
	void	addBuffer( std::string msg );
	void	cleanBuffer( void );
	void	nextStatus( void );
};

#endif