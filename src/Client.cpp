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

void		Client::setStatus(int status){
	_status = status;
}

int		Client::getStatus(){
	return DONE; //for debug, real is: return _status;
}

int	Client::sendInfo( void )
{
    int bytes = send(_fd, _sendInfo.c_str(), _sendInfo.length(), 0);
    if (bytes == -1)
        return (std::cerr << "Error en send " << std::endl, 0);
    return 1;
}

int	Client::receiveInfo( void )
{
	char buff[BUFF_SIZE];
    ssize_t bytes;

    while (true)
    {
        memset(buff, 0, sizeof(buff));
        bytes = recv(_fd, buff, sizeof(buff) - 1, 0); // Rabem dades.
        if (bytes <= 0)
            return (std::cout << "Client <" << _fd << "> Disconnected or Error occurred" << std::endl, 0);
        else
            _receiveInfo += std::string(buff);
    }
    return 1;
}