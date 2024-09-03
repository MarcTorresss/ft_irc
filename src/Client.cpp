#include "Client.hpp"

Client::Client(): _fd(-1), _ipAdd(""){
}

Client::Client(int fd, std::string ipadd): _fd(fd), _ipAdd(ipadd){
}

Client::~Client(){
}

std::string Client::getUserName()
{
    return _userName;
}

std::string Client::getNickName()
{
    return _nickName;
}

int Client::getFd()
{
	return _fd;
}

std::string Client::getIpAdd()
{
    return _ipAdd;
}

struct sockaddr_in Client::getClientAdd()
{
    return _clientAdd;
}

std::string Client::getSendInfo()
{
    return _sendInfo;
}

std::string Client::getReceiveInfo()
{
    return _receiveInfo;
}

void Client::setUserName( std::string& userName )
{
    _userName = userName;
}

void Client::setNickName( std::string& nickName )
{
    _nickName = nickName;
}

void Client::setFd( int fd )
{
	_fd = fd;
}

void Client::setIpAdd( std::string ipAdd )
{
    _ipAdd = ipAdd;
}

void Client::setClientAdd( struct sockaddr_in& clientAdd )
{
    _clientAdd = clientAdd;
}

void	Client::setSendInfo( std::string sendinfo )
{
	_sendInfo = sendinfo;
}

void	Client::setReceiveInfo( std::string reciveinfo )
{
	_receiveInfo = reciveinfo;
}

int	Client::sendInfo( void )
{
 	int ret = send(_fd, NULL, 0, 0);
    if (ret == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) // operacion de escritura bloqueada
            return 0;
        else
            return (std::cerr << "Error en send: " << strerror(errno) << std::endl, 0);
    }

    int bytes = send(_fd, _sendInfo.c_str(), _sendInfo.length(), 0);
    if (bytes == -1)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            return (std::cerr << "Error en send: " << strerror(errno) << std::endl, 0);
        return 0;
    }
    return 1; // El mensaje se ha enviado nishe
}

int	Client::receiveInfo( void )
{
	return 1;
}