#include "Client.hpp"

Client::Client(): _fd(-1), _status(0), _ipAdd(""), _inbuffer(""){
}

Client::Client(int fd, std::string ipadd): _fd(fd), _status(0), _ipAdd(ipadd){
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

std::string Client::getBuffer()
{
    return _buffer;
}

std::string Client::getinBuffer()
{
    return _buffer;
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

void	Client::setBuffer( std::string buffer )
{
	_buffer = buffer;
}

void	Client::setinBuffer( std::string buffer )
{
	_buffer = buffer;
}

void		Client::setStatus(int status){
	_status = status;
}

void    Client::nextStatus()
{
    if (_status == PASS)
    {
        _status = NICK;
    }
    else if (_status == NICK)
    {
        _status = USER;
    }
    else if (_status == USER)
    {
        _status = DONE;
    }
}

int		Client::getStatus(){
	return _status;
}

int	Client::sendInfo( void )
{
    int bytes = send(_fd, _buffer.c_str(), _buffer.length(), 0);
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
            _buffer += std::string(buff);
    }
    return 1;
}

void	Client::addBuffer( std::string msg )
{
    _buffer = _buffer + msg;
}

void	Client::addinBuffer( std::string msg )
{
    _buffer = _buffer + msg;
}

void	Client::cleanBuffer( void )
{
    // std::cout << "Buffer Cleaned" << std::endl;
    _buffer.clear();
}

void	Client::cleaninBuffer( void )
{
    // std::cout << "Buffer Cleaned" << std::endl;
    _inbuffer.clear();
}