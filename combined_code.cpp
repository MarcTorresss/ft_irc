#include "Channel.hpp"

Channel::Channel( Client *client,  std::string name, std::string pass)
{
	_userLimit = 5;
	_channelName = name;
	_channelPassword = pass;
	_admins.push_back(client->getNickName());
	_clients.push_back(client->getNickName());
}
std::string Channel::getName() const
{
	return _channelName;
}
std::string Channel::getPass() const
{
	return _channelPassword;
}

void	Channel::addClient( Client *client )
{
	if (_clients.size() >= _userLimit)
		throw std::runtime_error("Channel is full!");
	/*if (_inviteOnly && !isInvite(client))
		throw std::runtime_error("");*/
	_clients.push_back(client->getNickName());
}

void	Channel::addAdmin( Client *client, std::string target )
{
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	// if (!isClient(target))
		// throw std::runtime_error("That user is not connected to the channel");
	_admins.push_back(target);
}

void	Channel::setAdmin( std::string NickName )
{
	_admins.push_back(NickName);
}

int		Channel::someAdmin( void )
{
	return _admins.size();
}

void	Channel::removeAdmin( Client *client, std::string target )
{
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	unsigned long i = 0;
	for (i = 0; i < _admins.size(); ++i){
		if (_admins[i] == target)
			break;
	}
	std::vector< std::string>::iterator it = ( _admins.begin() + i );
	_admins.erase(it);
}

void	Channel::removeClient(Client *client, std::string target){
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	if (!isClient(client))
		throw std::runtime_error("That user is not connected to the channel");
	unsigned long i = 0;
	for (i = 0; i < _clients.size(); ++i){
		if (_clients[i] == target)
			break;
	}
	std::vector< std::string>::iterator it = ( _clients.begin() + i );
	_clients.erase(it);
}

void	Channel::addInvite( Client *client, std::string target )
{
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	_invites.push_back(target);
}

bool	Channel::isClient( Client *client )
{
	std::vector< std::string >::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
	{
    	if (*it == client->getNickName())
			return true;
	}
	return false;
}

bool	Channel::isAdmin( Client *client )
{
	std::vector< std::string >::iterator it;
    for (it = _admins.begin(); it != _admins.end(); ++it)
	{
    	if (*it == client->getNickName())
			return true;
	}
	return false;
}

bool	Channel::isInvite( Client *client )
{
	std::vector< std::string >::iterator it;
    for (it = _invites.begin(); it != _invites.end(); ++it)
	{
    	if (*it == client->getNickName())
			return true;
	}
	return false;
}

void	Channel::removeInvite( Client *client, std::string target)
{
	std::vector< std::string >::iterator it;
    for (it = _invites.begin(); it != _invites.end(); ++it)
	{
    	if (*it == target){
			if (isAdmin(client))
				_invites.erase(it);
		}
	}
}

void	Channel::setTopic( Client *client, std::string topic )
{
	if (_adminTopic && !isAdmin( client ))
		throw std::runtime_error("Not admin");
	_channelTopic = topic;
}

void Channel::setTopicAdmin(Client *client){
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	_adminTopic = !_adminTopic;
}

void Channel::setInviteOnly(Client *client){
	/*if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	_inviteOnly = !_inviteOnly;*/
	(void) client;
}

void	Channel::setPassword( Client *client, std::string pswd )
{
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	_channelPassword = pswd;
}

void	Channel::setUserLimit( Client *client, std::string limit )
{
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	if (limit.length() > 6)
		throw std::out_of_range("User limit not allowed (max. 999999)");
	for (int i = 0; limit[i] != 0; ++i){
		if (!isdigit(limit[i]))
			throw std::out_of_range("User limit must be a number (max. 999999)");
	}
	if (limit.length() == 0)
		_userLimit = -1;
	int lim = atoi(limit.c_str());
	if (lim < 0)
		throw std::out_of_range("User limit must be a positive int (max. 999999)");
	_userLimit = lim;
}

std::vector< std::string >	Channel::getClients()
{
	return _clients;
}

void Channel::setName(std::string name){_channelName = name;}

Channel::~Channel()
{

}#include "Client.hpp"

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
}#include "ircserv.hpp"

Server::Server(): _serSocketFd(-1), _port(4444){
}

Server::Server(int port, std::string pass ): _serSocketFd(-1), _port(port), _password(pass) {
}

Server::~Server(){
}

void Server::loop()
{
	while (!serverShutdown)
	{
		int	revents;
		int pollResult = poll(&_fds[0],_fds.size(), -1);
    	if (pollResult == -1 && !serverShutdown)
		{
        	throw std::runtime_error("The function poll() failed");
		}
		for (size_t i = 0; i < _fds.size(); i++)
		{
			revents = _fds[i].revents;
        	// No hay eventos, saltar a la siguiente iteración.
			if (revents != 0) 
			{
				// Manejo de errores o desconexiones inesperadas.
				if ((revents & POLLERR) == POLLERR || (revents & POLLHUP) == POLLHUP)
				{
					std::cout << "Socket error or client disconnection\n";
					clearClients(_fds[i].fd);
				}
				else if (revents & POLLIN) // Hay datos para leer.
				{
					if (_fds[i].fd == _serSocketFd) // Nueva conexión entrante.
					{
						acceptNewClient();
					}
					else // Mensaje de un cliente existente.
					{
						receiveNewData(_fds[i].fd);
					}
					// Marcar todos los fds como listos para escribir.
					for (size_t j = 1; j < _fds.size(); j++) 
					{
						_fds[j].events |= POLLOUT;
					}
				}
				else if (_fds[i].revents & POLLOUT) // Listo para enviar datos.
				{
					if (_fds[i].fd != _serSocketFd)
					{
						Client* cli = getClient((int)_fds[i].fd);
						if (cli)
						{
							if (cli->sendInfo())
							{
								cli->cleanBuffer();
							}
							else
							{
								disconnectClient(cli, std::string("client disconnection: " + cli->getNickName() + "\n"), true);
							}
						}
					}
					// Limpiar el evento POLLOUT para evitar más escrituras innecesarias.
					_fds[i].events &= ~POLLOUT;
				}
        	}
		}
	//closeFds();
	}
}

void Server::createSocket()
{
	struct	sockaddr_in sockAddr;
	struct	pollfd		serPoll;

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(_port);
	sockAddr.sin_addr.s_addr = INADDR_ANY;
	std::memset(sockAddr.sin_zero, 0, sizeof(sockAddr.sin_zero));

	_serSocketFd = socket(AF_INET, SOCK_STREAM, 0); //comunicacion ipv4 por tcp, protocolo variable segun el sistema
	if (_serSocketFd == -1)
		throw(std::logic_error("failed to create socket"));
	int en = 1;
	if(setsockopt(_serSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) // configuramos el socket con SO_REUSEADDR para poder reutilizar puertos o ips
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
	if (fcntl(_serSocketFd, F_SETFL, O_NONBLOCK) == -1) // funciones como read(), write(), accept(), no se quedaran colgadas volveran con un error
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on socket"));
	if (bind(_serSocketFd, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == -1) // asignamos una ip y un puerto
		throw(std::runtime_error("failed to bind socket"));
	if (listen(_serSocketFd, SOMAXCONN) == -1) // vinculamos el fd y configuramos la cantidad maxima de solicitudes de conexion
		throw(std::runtime_error("listen() failed"));

	serPoll.fd = _serSocketFd; //el port d'escolta del serversocket
	serPoll.events = POLLIN;  // poll() nos dira si hay datos para leer en fd o nuevas conexiones en un socket de servidor 
	_fds.push_back(serPoll); //afagir el pollfd al vector

	std::cout << GRE << "Server <" << _serSocketFd << "> Connected" << WHI << std::endl << "";
	std::cout << "Waiting for clients..." << std::endl;
}

void Server::acceptNewClient()
{
	std::cout << GRE << "NEW CLIENT" << WHI << std::endl;
	Client cli; //-> create a new client
	struct sockaddr_in cliSocket;//for storing IP, PORT, PROT
	struct pollfd newPoll;
	socklen_t len = sizeof(cliSocket);

	int cliFd = accept(_serSocketFd, (sockaddr *)&cliSocket, &len); //-> accept the new client & store sock info in cliSocket
	if (cliFd == -1)
	{
		std::cout << "accept() failed" << std::endl;
		return; //throw?
	}
	// std::string capResponse = ":localhost CAP * LS :\r\n";
    // send(cliFd, capResponse.c_str(), capResponse.size(), 0);

	if (fcntl(cliFd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
	{
		std::cout << "fcntl() failed" << std::endl;
		return;
	}

	newPoll.fd = cliFd; //-> add the client socket to the pollfd
	newPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	newPoll.revents = 0; //-> set the revents to 0

	cli.setFd(cliFd); //-> store the client file descriptor
	cli.setIpAdd(inet_ntoa((cliSocket.sin_addr))); //-> convert the ip address to string and set it
	_clients.push_back(cli); //-> add the client to the vector of clients
	_fds.push_back(newPoll); //-> add the client socket to the pollfd

    // std::string welcome = cli.getNickName() + "Welcome to the IRC server!\r\n";
    // send(cliFd, welcome.c_str(), welcome.size(), 0);
	std::cout << GRE << "Client <" << cliFd << "> Connected" << WHI << std::endl;
}

Client* Server::getClient(int fd) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getFd() == fd) {
            return &(*it);
        }
    }
    return NULL;
}

int	Server::getChannelIndex(){
	return 0; //DEBE RETORNAR EL INDICE DEL ARRAY DEL CANAL DONDE SE HA ENVIADO EL MENSAJE
}

void Server::clearClients(int fd){
	for(size_t i = 0; i < _fds.size(); i++){ //-> remove the client from the pollfd
		if (_fds[i].fd == fd)
			{_fds.erase(_fds.begin() + i); break;}
	}
	for(size_t i = 0; i < _clients.size(); i++){ //-> remove the client from the vector of clients
		if (_clients[i].getFd() == fd)
			{_clients.erase(_clients.begin() + i); break;}
	}
}

void	Server::receiveNewData(int fd)
{
	std::cout << GRE << "DATA" << WHI << std::endl;
  	char buff[BUFF_SIZE];
	Client	*cli = getClient(fd);

    memset(buff, 0, BUFF_SIZE);
    ssize_t bytes = recv(fd, buff, BUFF_SIZE - 1, 0);

    if (bytes < 0)
	{
        std::cerr << "Error en recv para el cliente <" << fd << ">" << std::endl;
        clearClients(fd);
        close(fd);
    }
    else if (bytes == 0)
	{
        std::cout << RED << "Cliente <" << fd << "> Desconectado" << WHI << std::endl;
        clearClients(fd);
        close(fd);
    }
    else 
	{
        buff[bytes] = '\0';
        std::cout << YEL << "Cliente <" << fd << "> Datos: " << WHI << buff << std::endl;
		cli->addinBuffer(buff);
		check_comand(cli);
    }
}

void	Server::check_comand(Client *cli )
{
    std::string command;
    std::string space;
	std::string flag = "\n";
	std::vector<std::string> params;
	std::string temp = "";

	if (cli->getinBuffer().find(std::string("\r\n")) != std::string::npos
		|| cli->getinBuffer().find(std::string("\n")) != std::string::npos)
	{
			std::string receivedData(cli->getinBuffer());
			cli->cleaninBuffer();
    		std::istringstream iss(receivedData);
		// Este bucle procesa las líneas de mensajes desde el cliente.
		while (!receivedData.empty())
		{
			if (receivedData.find("\r\n") != std::string::npos)
			{
				flag = "\r\n";
			}
			std::cout << "The data:" << receivedData << "." << std::endl;
			// Buscar el delimitador de fin de línea
			size_t pos = receivedData.find(flag);
			std::string sms;

			// Si se encuentra el delimitador, extraer el mensaje
			if (pos != std::string::npos)
			{
				sms = receivedData.substr(0, pos);
				receivedData.erase(0, pos + 2); //Eliminamos mansaje de la linia
			}
			else
			{
				// Si no se encuentra el delimitador, se asume que queda un mensaje parcial
				sms = receivedData; 
				receivedData = ""; // Limpiar la línea para salir del bucle
			}

			// std::cout << "Msg from " << cli->getNickName() << "😈😈😈😈😈"  << ": " << sms << std::endl;
			params.clear();
			std::istringstream iss(sms);
			std::string token, command;

			if (sms[0] == ':')
			{
				iss >> token; // Ignorar el prefijo (si existe)
			}

			// Leer el comando principal
			iss >> command;

			while (iss >> token)
			{
				// Si el token comienza con ':', se trata de un "trailing"
				if (token[0] == ':')
				{
					std::string trailing;
					std::getline(iss, trailing); // Leer el resto de la línea
					params.push_back(token.substr(1) + trailing); // Agregar el "trailing" sin el ':'
					break; // Salir del bucle al encontrar el "trailing"
				}
				else
				{
					// std::cout << "The data:" << token << "." << std::endl;
					params.push_back(token); // Agregar el token a los parámetros
				}
			}
			if (!params.empty())
			{
				std::string& lastParam = params[params.size() - 1]; // Obtener una referencia al último parámetro

				// Verificar si el último parámetro no está vacío y termina con '\r'
				if (!lastParam.empty() && lastParam[lastParam.size() - 1] == '\r')
				{
					lastParam.resize(lastParam.size() - 1); // Eliminar el último carácter
				}
			}
			int i = 0;
			for (i = 0; command[i] != 0; ++i){
				temp += (char)toupper(command[i]);}
			command = temp;
			temp.clear();
			std::string commands[] = { "NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE", "PING", "PASS", "WHOIS", "QUIT"};
			for (i = 0; i < 10; i++) {
				if (commands[i] == command)
					break;
			}
			switch (i)
			{
				case 0: // NICK
					_setNickname(cli, params);
					break;
				case 1: // USER
					_setUser(cli, params);
					break;
				case 2: // JOIN
					_handleJoin(cli, params);
					break;
				case 3: // PRIVMSG
					_handlePrivmsg(cli, params);
					break;
				case 4: // KICK
					_handleKick(cli, params);
					break;
				case 5: // INVITE
					_handleInvite(cli, params);
					break;
				case 6: // TOPIC
					_handleTopic(cli, params);
					break;
				case 7: // MODE
					_handleMode(cli, params);
					break;
				case 8: // PING
					_handlePing(cli, params);
					break;
				case 9: // PASS
					_authenticatePassword(cli, params);
					break;
				case 10: // WHOIS
					_handleWhoIs(cli, params);
					break;
				// case 11: // QUIT
					//_quitChannel(cli, params);
					// break;
				default:
					cli->addBuffer(ERR_UNKCMD421);
					std::cout << "Comando no reconocido: " << command << std::endl;
					break;
			}
		}
	}
}

void Server::infoAllServerClients( std::string msg )
{
	for (size_t i = 0; i < _clients.size(); i++)
		_clients[i].addBuffer(msg);
}

Client	*Server::getClientNickName( std::string NickName )
{
	std::vector<Client>::iterator it = _clients.begin();
	for (; it != _clients.end(); ++it)
		if ((*it).getNickName() == NickName)
			return &(*it);
	return NULL;
}

const std::vector<Channel>& Server::getChannels() const {
    return _channels;
}#include "ircserv.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void	Server::_setNickname(Client *cli, std::vector<std::string> params){
	// NO PUEDEN AVER NICKS REPETIDOS
	if (params.empty())
	{
		// cli->setNickName(params[0]);
		std::cout << "client " << cli->getFd() << " NICK is: " << cli->getNickName() << "-" << std::endl;
	}
	if (cli->getStatus() >= PASS){
		std::cout << "NICK changed" << std::endl;
		cli->setNickName(params[0]);
	}
	else{
		std::cout << ERR << "cannot change nick if password is not set" << std::endl;
		//send message to client
	}
	cli->nextStatus();
}

void	Server::_setUser(Client *cli, std::vector<std::string> params){
	if (params.empty()){
		std::cout << "client <" << cli->getFd() << "> USR is: " << cli->getUserName() << "-" << std::endl;
	}
	if (cli->getStatus() >= NICK){
		std::cout << "USR changed" << std::endl;
		cli->setUserName(params[0]);
	}
	else{
		std::cout << ERR << "cannot change username if nickname is not set" << std::endl;
		//send err message to client
	}
	handleConnection(cli); //esto no va aqui
	cli->nextStatus();
}

void	Server::_handlePrivmsg(Client *cli, std::vector<std::string> params)
{
	(void) params;
    (void) cli;
	/*if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
 	// destinatario y mensaje
    if (params.size() < 2)
    {
        cli->addBuffer(ERR_PARAM461);
        return;
    }

    std::string target = params[0]; // El destinatario (cliente o canal)
    std::string message = params[1]; // El mensaje (puedes unir los params si hay espacios)

    // Comprobar si el destinatario es un cliente o un canal
    if (isChannel(target))
    {
        // Enviar mensaje a todos los usuarios del canal
        Channel *channel = findChannel(target);
        if (channel)
        {
            channel->broadcastMessage(cli, message);
        }
        else
        {
            cli->addBuffer("401 " + target + " :No such channel\r\n");
        }
    }
    else
    {
        // Enviar mensaje a un cliente específico
        Client *recipient = findClient(target);
        if (recipient)
        {
            recipient->addBuffer(":" + cli->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n");
        }
        else
        {
            cli->addBuffer("401 " + target + " :No such nick\r\n");
        }
    }*/
}

void	Server::_handleKick(Client *cli, std::vector<std::string> params){
	if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{
		int channelIdx = getChannelIndex();
		_channels[channelIdx].removeClient(cli, params[0]);
	}
}

void	Server::_handleInvite(Client *cli, std::vector<std::string> params){
	if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{
		int channelIdx = getChannelIndex();
		//does the client need to be connected to server to be invited??
		_channels[channelIdx].addInvite(cli, params[0]);
		//2. notify user?
	}
}

void	Server::_handleTopic(Client *cli, std::vector<std::string> params){
	if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{	
		int channelIdx = getChannelIndex();
		_channels[channelIdx].setTopic(cli, params[0]);
	}
}

void	Server::_handleMode(Client *cli, std::vector<std::string> params){
	/*if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{*/
		for (size_t i = 0; i < params.size(); ++i) {
			std::cout << "Param " << i << ": " << params[i] << std::endl;
		}
		std::string type = params[params.size() - 1];
		std::cout << type << std::endl;
		if (params.empty()){
			std::cout << "the current channel modes are [" << "]" <<std::endl; //print los channel modes
		}
		std::string modes[] = {"+i","t","k","o","l"};
		int i = 0;
		for (i = 0; i < 5; ++i){
			// std::cout << params[params.size() - 1] << std::endl;
			// for (size_t i = 0; i < params.size(); ++i) {
			// 	std::cout << "Param " << i << ": " << params[i] << std::endl;
			// }
			// if (modes[i] == params[params.size() - 1])
			// 	break;
			if (modes[i] == type)
				break;
		}
		try{
			switch (i) //DONE
			{
				case 0: //i
					_channels[0].setInviteOnly(cli);
					break;
				case 1: //t
					_channels[0].setTopicAdmin(cli);
					break;
				case 2: //k
					_channels[0].setPassword(cli,params[0]);
					break;
				case 3: //o
					_channels[0].addAdmin(cli,params[0]);
					break;
				case 4: //l
					_channels[0].setUserLimit(cli,params[0]);
					break;
				default:
					std::cout << ERR << "Channel MODE not existent [i, t, k, o, l]" <<std::endl;
					break;
			}
		}catch(std::exception &e){
			std::cout << ERR << e.what() <<std::endl;
		}
	//}
}

void	Server::_handlePing(Client *cli, std::vector<std::string> params)
{
    (void) params;
	if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" <<std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{
		std::cout << "test" << std::endl;
    	cli->addBuffer("Ping Marc Pong Albert and javi from munich\r\n");
	}
}

void Server::addChannel(Client *cli, const std::string& channelName, const std::string& password) {
    Channel newChannel(cli, channelName, password);
    _channels.push_back(newChannel);
    getChannelsList();
}

std::vector<std::string> splitString(const std::string& input, char delimiter)
{
    std::istringstream stream(input);
    std::string token;
    std::vector<std::string> ret;

    while (std::getline(stream, token, delimiter))
        ret.push_back(token);
    
    return ret;
}

void Server::_handleJoin(Client *cli, std::vector<std::string> params)
{
	if (cli->getStatus() != DONE)
	{
		std::cout << "User not registred!" << std::endl; 
		cli->addBuffer("User not registred!\r\n");
	}
	else
	{
		//param format [#channel1,#channel2 key1,key2]
		std::vector<std::pair<std::string, std::string> > channelKeyPairs;    std::vector<std::string> channels, keys;
		std::istringstream namechanel(params[0]);
		std::istringstream passchanel(params[1]);
		std::string channIn, keysIn;

		std::cout << params[0] << std::endl;
		std::getline(namechanel, channIn, ' ');
		std::cout << "Channels: " << channIn << std::endl;
		std::getline(passchanel, keysIn);
		std::cout << "Keys: "<< keysIn << std::endl;


		if (channIn.empty() || keysIn.empty())
		{
			std::cout << "Invalid JOIN format, #channel key or [#channel1,#channel2] [key1,key2]" << std::endl;
			cli->addBuffer("Invalid JOIN format, #channel key or [#channel1,#channel2] [key1,key2]\r\n");
			return;
		}
		channels = splitString(channIn, ',');
		keys = splitString(keysIn, ',');

		if (channels.size() != keys.size())
			{std::cout << "Number of channels does not match number of keys" << std::endl; return;}   
		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i].find("#") != 0)
				{std::cout << "Invalid channel format. Channels must start with '#'." << std::endl; return;}
			channelKeyPairs.push_back(std::make_pair(channels[i], keys[i]));
			const std::string& channelName = channelKeyPairs[i].first;
			const std::string& channelKey = channelKeyPairs[i].second;
			validateChannelPassword(cli, channelName, channelKey);
		}
		//parse channel and pass
		//if channel exists
			//check password
				//join client
		//else addChannel
	}
}

void Server::_authenticatePassword(Client *cli, std::vector<std::string> params)
{
	if (cli->getStatus() != PASS)
	{
		disconnectClient( cli, "462 * :" + params[0] + " may not reregister (need PASS state)\r\n", false );
	}
	else if (params[0].size() < 1)
	{
		cli->addBuffer("461 " + cli->getNickName() + " PASS :Parameters missing\r\n");
	}
	else if (params[0] != _password)
	{
		cli->addBuffer("464 * :Password " + params[0] + " incorrect " + _password + "\r\n");
	}
	else
	{
		cli->nextStatus();
	}
}

void	Server::_handleWhoIs(Client *cli, std::vector<std::string> params)
{
	(void) cli;
	(void) params;
}

bool Server::validateChannelPassword(Client *cli, const std::string& channelName, const std::string& password) {
    
    for (size_t i = 0; i < _channels.size(); i++) {
        if (_channels[i].getName() == channelName) {
            // If the channel exists, validate the password
            if (_channels[i].getPass() == password) {
                std::cout << "Password is valid for channel: " << channelName << std::endl;
                _channels[i].addClient(cli);
                return true;
            } else {
                std::cout << "Invalid password for channel: " << channelName << std::endl;
                return false;
            }
        }
    }
	std::string channelAdded = "Channel " + channelName + " added!\r\n";
    send(cli->getFd(), channelAdded.c_str(), channelAdded.size(), 0);
    addChannel(cli, channelName, password);
    return true;
}

void Server::disconnectClient(Client *client, std::string msg, bool sendmsg)
{
	if (sendmsg)
	{
		std::string disconnect_msg = ":" + client->getNickName() + "!~" + client->getUserName() + " QUIT :" + msg + " \r\n";
		infoAllServerClients(disconnect_msg);
	}
    // Iterar sobre los canales para los que el cliente está registrado
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].isClient(client))
        {
			_channels[i].removeClient(client, client->getNickName());
			// Si el canal queda vacío, lo eliminamos
			if (_channels[i].getClients().size() == 0)
			{
				_channels.erase(_channels.begin() + i);
				i--;
			}
			else
            {
                // Si no hay administradores en el canal, asignar un nuevo administrador
                if (_channels[i].someAdmin() == 0)
                {
                    // Obtener el primer cliente del canal
                    std::vector<std::string> AllClients = _channels[i].getClients();
                    Client* newAdmin = getClientNickName(AllClients[0]);

                    // Asignar el nuevo administrador
                    _channels[i].setAdmin(newAdmin->getNickName());

                    // Notificar al nuevo administrador
                    std::string newAdminText = _channels[i].getName() + " No admins left connected/you are the new Admin \r\n";
                    newAdmin->addBuffer(newAdminText);
                }
            }
        }
    }
    clearClients(client->getFd());
}

void Server::handleConnection(Client *client)
{
	std::string serverName = "MyIRC";
	std::string version = "1.0";
    // Enviar mensaje de bienvenida a Irssi
	std::string welcomeMessage = ":" + serverName + " 001 " + client->getNickName() + " :Welcome to the " + serverName + " IRC Network, " + client->getNickName() + "\r\n";
	client->addBuffer(welcomeMessage);

	// Enviar información sobre el servidor
	std::string versionMessage = ":" + serverName + " 002 " + client->getNickName() + " :Your host is " + serverName + ", running version " + version + "\r\n";
    client->addBuffer(versionMessage);
}
#include "ircserv.hpp"
#include "utils.cpp"

bool serverShutdown = false;

int main( int argc, char **argv)
{
	signal(SIGINT, signalHandler);
	signal(SIGQUIT, signalHandler);
	if (check_arguments(argc, argv) == 0) 
	{
		Server server(atoi(argv[1]), argv[2]);
		
		try
		{
			server.createSocket();
			server.loop();
		}
		catch(std::exception &e)
		{
			std::cout << "Exception caught: " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << "Usage is ./ircserv [port] [password]" << std::endl;
		return -1;
	}
}#include "Server.hpp"
#include "ircserv.hpp"

void signalHandler(int signum) {
	(void)signum;
	serverShutdown = true;
	//std::cout << "Signal caught: " << signum << std::endl;
}

int check_arguments(int argc, char **argv) {
	if (argc != 3){
		std::cerr << ERR <<"Wrong arguments [port, password]" << WHI << std::endl;
		return -1;
	}
	int i = 0;
	for (i=0; argv[1][i] == '0'; ++i);
	std::string port(&argv[1][i]);
	//std::cout << WHI << "PORT IS: " << port << RED << std::endl;
	if (port.length() > 6){
		std::cerr << ERR <<"Invalid Port" << std::endl;
		return -1;
	}
	for (int i = 0; port[i] != 0; ++i){
		if (!isdigit(port[i])){
			std::cerr << ERR <<"Port must be a number" << WHI << std::endl;
			return -1;
		}
	}
	int portnum = atoi(argv[1]);
	//es poden utilitzar del 49152 al 65535?
	if (portnum > 65535){
		std::cerr << ERR <<"Port number too big (max is 65535)" << WHI<< std::endl;
		return -1;
	}
	//0 - 1024 reservat per http, ssh,...
	if (portnum < 1024){
		std::cerr << ERR <<"Port number is reserved: use within 1024-65535 range" << WHI<< std::endl;
		return -1;
	}
	std::string password(argv[2]);
	if (password.length() > 30){ 
		std::cerr << ERR <<"Password is too long (max 30 chars)" << WHI<< std::endl;
		return -1;
	}
	return 0;
}

void Server::getChannelsList() const {
	
	const std::vector<Channel>& channels = getChannels();

    if (channels.empty()) {
        std::cout << "No channels available." << std::endl;
        return;
    }

    std::cout << "List of Channels: " << std::endl;
    for (size_t i = 0; i < channels.size(); ++i) {
        std::cout << "Channel " << i + 1 << ": " << std::endl;
        std::cout << "  Name: " << channels[i].getName() << std::endl;
        std::cout << "  Password: " << (channels[i].getPass().empty() ? "No password" : channels[i].getPass()) << std::endl;
    }
}#ifndef CHANNEL_HPP_
# define CHANNEL_HPP_

# include "ircserv.hpp"
#include <iostream>
#include <string>
#include <sstream>

class Client;

class Channel
{
private:
	std::string	_channelName;
	std::string	_channelPassword;
	std::string	_channelTopic;

	bool	_adminTopic;
	bool	_inviteOnly;
	unsigned long	_userLimit;
    std::vector< std::string > _clients;
	std::vector< std::string > _admins;
    std::vector< std::string > _invites;

public:
	Channel();
	Channel( Client *client, std::string name, std::string pass);

	~Channel();

	bool	isClient( Client *client );
	bool	isAdmin( Client *client );
	bool	isInvite( Client *client );

	void	addAdmin( Client *client, std::string target );
	void	addInvite( Client *client, std::string target );
	void	addClient( Client *client);
	void	removeAdmin( Client *client, std::string target );
	void	removeInvite( Client *client, std::string target );
	void	removeClient(Client *client, std::string target);

	std::string getName() const;
	std::string getTopic() const;
	std::string getPass() const;

	void setUserLimit( Client *client, std::string limit );
	void setName(std::string name);
	void setPassword(Client *client, std::string pswd );
	void setInviteOnly(Client *client);
	void setTopic( Client *client, std::string topic);
	void setTopicAdmin(Client *client);
	int someAdmin( void );
	void setAdmin( std::string NickName );
	std::vector< std::string >	getClients();
};

#endif#ifndef CLIENT_HPP_
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
	std::string			_inbuffer;

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
	std::string getinBuffer( void );
	void	setinBuffer( std::string inbuffer );
	void	addinBuffer( std::string msg );
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
	void	cleaninBuffer( void );
	void	nextStatus( void );
};

#endif#ifndef SERVER_HPP_
# define SERVER_HPP_

#include "ircserv.hpp"
#include "Channel.hpp"
#include <iostream>
#include <string>
#include <sstream>

# define BUFF_SIZE 1024

class Channel;
class Client;

class Server
{
private:
	int _serSocketFd;
	int _port;
	std::vector<Client> _clients;
	std::vector<Channel> _channels;
	std::vector<struct pollfd> _fds;
	std::string _password; //pass del server o del cliente que te pasa

    Client* getClient(int fd);

	void	_authenticatePassword(Client *cli, std::vector<std::string> params);
    void	_setNickname(Client *cli, std::vector<std::string> params);
    void	_setUser(Client *cli, std::vector<std::string> params);
    void	_sendPing(Client *cli, std::vector<std::string> params);
    void	_handleCapabilities(Client *cli, std::vector<std::string> params);
    void	_handleQuit(Client *cli, std::vector<std::string> params);
    void	_handleJoin(Client *cli, std::vector<std::string> params);
    void	_handlePrivmsg(Client *cli, std::vector<std::string> params);
    void	_handleKick(Client *cli, std::vector<std::string> params);
    void	_handleInvite(Client *cli, std::vector<std::string> params);
    void	_handleTopic(Client *cli, std::vector<std::string> params);
    void	_handleMode(Client *cli, std::vector<std::string> params);
	void	_handlePing(Client *cli, std::vector<std::string> params);
	void	_handleWhoIs(Client *cli, std::vector<std::string> params);

public:
	Server();
	Server( int port, std::string pass );
	~Server();

	// static void signalHandler(int signum);
	void		createSocket();
	void 		loop();
	void		acceptNewClient();
	void		receiveNewData(int fd);
	void		check_comand(Client *cli );

	void 		closeFds();
	void 		clearClients(int fd);
	int			getChannelIndex(); //DEBE RETORNAR EL INDICE DEL ARRAY DEL CANAL DONDE SE HA ENVIADO EL MENSAJE
	void 		addChannel(Client *cli, const std::string& channelName, const std::string& password);
	bool 		validateChannelPassword(Client *cli, const std::string& channelName, const std::string& password);
  	void		disconnectClient(Client *client, std::string msg, bool sendmsg);
	void		infoAllServerClients( std::string msg );
	Client		*getClientNickName( std::string NickName );
	void		handleConnection(Client *client);

	//TO DO
	bool		isChannel( std::string nameChannel );
	Channel		*findChannel( std::string nameChannel );
	Client		*findClient( std::string nick );
	void		broadcastMessage( Client cli , std::string message);

	//Debugging
	const std::vector<Channel>& getChannels() const;
	void getChannelsList() const;
};

#endif#ifndef IRCSERV_HPP_
# define IRCSERV_HPP_

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <map>

//-------------------------COLORS------------------------------//
#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

#define ERR "\e[1;31mError: "

//---------------------------CLASSES---------------------------//
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

//----------------------------ERRORS---------------------------//

#define ERR_CHANN442 "442 * " + params[0] + " :Not a member of the channel\r\n" // Intentar interactruar con un canal que no esta
#define ERR_OPNEEDED "482 * " + cli->getNickName() + " :Operator privileges are required for this action\r\n" // El cliente no tiene permisos para canviar cosas del canal
#define ERR_PARAM461 "461 " + cli->getNickName() + " :Parameters missing\r\n" // Faltan parametros al ejecutar un comando
#define ERR_TOPIC331 "331 " + params[0] + " :No channel topic set\r\n"
#define ERR_NICKN401 "401 :Invalid nickname or channel " + params[0] + " for " + params[1] + "\r\n"
#define ERR_INCHA443 "443 " + params[1] + " " + params[0] + " :User already in the channel\r\n" // Invitar un usuario que ya esta en el chanel
#define ERR_NOCHANEL "403 " + params[0] + " " + cli->getNickName() + " :Channel not found\r\n" // Interactuar con un channel que no existe
#define MSG_TOPIC332 "332 " + params[0] + " :Current topic is: " + channel->getTopic() + "\r\n"
#define ERR_UNKCMD421 "421 " + cli->getNickName() + " :Unknown command or invalid parameters\r\n"

enum _statusClient
{
	PASS = 0,
	NICK,
	USER,
	DONE
};

//----------------------------UTILS----------------------------//
extern bool serverShutdown;

void signalHandler(int signum);
int check_arguments(int argc, char **argv);

#endifNAME	 =	ircserv
FLAGS	 =	-std=c++98 -Wall -Wextra -Werror
INCLUDE  =	-Iinc/
DIR_OBJS =	objs/
DIR_SRC  = src/

# *******************************	FILES	******************************* #

FILES		=	main.cpp Server.cpp Client.cpp Channel.cpp utils.cpp commands.cpp
FILES_SRC	=	$(addprefix $(DIR_SRC),$(FILES))

# *******************************  OBJECTS	******************************* #

OBJS	=	$(FILES_SRC:%.cpp=$(DIR_OBJS)%.o) #$(addprefix $(DIR_OBJS),$(FILES_SRC:.cpp=.o))
DEPS	=	$(OBJS:%.o=%.d) #$(OBJS:.o=.d)

# *******************************  COLORS	******************************* #

RED			=	\033[0;31m
GREEN		=	\033[0;32m
YELLOW		=	\033[0;33m
BLUE		=	\033[0;34m
PURPLE		=	\033[0;35m
CYAN		=	\033[0;36m
RESET		=	\033[0m
GREEN_BOLD	=	\033[1;32m
BLUE_BOLD	=	\033[1;34m
CYAN_BOLD	=	\033[1;36m

# ********************************  RULES   ******************************* #

all: $(DIR_OBJS) $(NAME)

$(DIR_OBJS):
	@mkdir -p $(DIR_OBJS)

$(NAME): $(OBJS)
	c++ $(FLAGS) $(INCLUDE) $(OBJS) -o $(NAME)
	@echo "${BLUE_BOLD}Internet Relay Chat ${GREEN}compiled ✅\n${RESET}"

$(DIR_OBJS)%.o: %.cpp Makefile
	@mkdir -p $(dir $@)
	c++ $(FLAGS)  $(INCLUDE) -MMD -c $< -o $@
	@echo "${YELLOW}Compiling ${RESET}$@...${RESET}"

clean:
	@rm -rf $(DIR_OBJS)
	@echo "${RED}Deleting${RESET} all objects 🗑"

fclean: clean
	@rm -f $(NAME)
	@echo "${BLUE_BOLD}Internet Relay Chat ${RED}deleted${RESET}"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re