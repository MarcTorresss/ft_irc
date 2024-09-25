#include "Channel.hpp"

Channel::Channel( Client *client )
{
	_admins.push_back(client->getNickName());
	_clients.push_back(client->getNickName());
}
std::string Channel::getName()
{
	return _channelName;
}
std::string Channel::getPass()
{
	return _channelPassword;
}

void	Channel::addClient( Client *client )
{
	if (_clients.size() >= _userLimit)
		throw std::runtime_error("Channel is full!");
	if (_inviteOnly && !isInvite(client))
		throw std::runtime_error("User was not invited");
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
	if (!isAdmin( client ))
		throw std::runtime_error("Not admin");
	_inviteOnly = !_inviteOnly;
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

std::string Client::getBuffer()
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

void		Client::setStatus(int status){
	_status = status;
}

int		Client::getStatus(){
	return DONE; //for debug, real is: return _status;
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

void	Client::cleanBuffer( void )
{
    std::cout << "aaaaaa" << std::endl;
    _buffer.clear();
}#include "ircserv.hpp"

Server::Server(): _serSocketFd(-1), _port(4444){
}

Server::Server(int port): _serSocketFd(-1), _port(port){
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
								disconnectClient(cli, std::string("client disconnection: " + cli->getNickName() + "\n"));
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
	std::string capResponse = ":localhost CAP * LS :\r\n";
    send(cliFd, capResponse.c_str(), capResponse.size(), 0);

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

    std::string welcome = cli.getNickName() + "Welcome to the IRC server!\r\n";
    send(cliFd, welcome.c_str(), welcome.size(), 0);
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
		check_comand(buff, cli);
    }
}

void	Server::check_comand( char *buff, Client *cli )
{
	std::string receivedData(buff);
    std::istringstream iss(receivedData);
    std::string command;
    std::string params;
	std::string temp = "";

    // Separar el comando de los parámetros
    iss >> command;
    std::getline(iss, params);

	int i = 0;
	for (i = 0; command[i] != 0; ++i){
		temp += (char)toupper(command[i]);}
	command = temp;
    std::string commands[] = { "NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE", "PING", "PASS", "QUIT"};
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
            //_authenticatePassword(cli, params);
            break;
		case 10: // QUIT
            //_quitChannel(cli, params);
            break;
        default:
			cli->addBuffer(ERR_UNKCMD421);
            std::cout << "Comando no reconocido: " << command << std::endl;
            break;
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
}#include "ircserv.hpp"

void	Server::_setNickname(Client *cli, std::string& params){
	if (params == ""){
		std::cout << "client " << cli->getFd() << " NICK is: " << cli->getNickName() << "-" << std::endl;
	}
	if (cli->getStatus() >= PASS){
		std::cout << "NICK changed" << std::endl;
		cli->setNickName(params);
	}
	else{
		std::cout << ERR << "cannot change nick if password is not set" << std::endl;
		//send message to client
	}
}

void	Server::_setUser(Client *cli, std::string& params){
	if (params == ""){
		std::cout << "client <" << cli->getFd() << "> USR is: " << cli->getUserName() << "-" << std::endl;
	}
	if (cli->getStatus() >= NICK){
		std::cout << "USR changed" << std::endl;
		cli->setUserName(params);
	}
	else{
		std::cout << ERR << "cannot change username if nickname is not set" << std::endl;
		//send err message to client
	}
}

void	Server::joinChannel(Client *cli, std::string& params){
	(void) cli;
	(void) params;
	std::cout << "client <" << cli->getFd() << "> wants to join channel " << params << std::endl;
	for (int i = 0; _channels.size(); ++i){
		if (_channels[i].getName() == params)
			_channels[i].addClient(cli);//join channel + check channel perms (password, invite only, has been invited?)
	}
	//else create channel
}

void	Server::_handlePrivmsg(Client *cli, std::string& params){
	(void) cli;
	(void) params;
	//create a private(invite only) channel between 2 clients
}

void	Server::_handleKick(Client *cli, std::string& params){
	int channelIdx = getChannelIndex();
	_channels[channelIdx].removeClient(cli, params);
}

void	Server::_handleInvite(Client *cli, std::string& params){
	int channelIdx = getChannelIndex();
	//does the client need to be connected to server to be invited??
	_channels[channelIdx].addInvite(cli, params);
	//2. notify user?
}

void	Server::_handleTopic(Client *cli, std::string& params){
	int channelIdx = getChannelIndex();
	_channels[channelIdx].setTopic(cli, params);
}

void	Server::_handleMode(Client *cli, std::string& params){
	if (params == ""){
		std::cout << "the current channel modes are [" << "]" <<std::endl; //print los channel modes
	}
	std::string modes[] = {"i","t","k","o","l"};
	int i = 0;
	for (i = 0; i < 5; ++i){
		if (modes[i] == params)
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
				_channels[0].setPassword(cli,params);
				break;
			case 3: //o
				_channels[0].addAdmin(cli,params);
				break;
			case 4: //l
				_channels[0].setUserLimit(cli,params);
				break;
			default:
				std::cout << ERR << "Channel MODE not existent [i, t, k, o, l]" <<std::endl;
				break;
		}
	}catch(std::exception &e){
		std::cout << ERR << e.what() <<std::endl;
	}
}

void	Server::_handlePing(Client *cli, std::string& params)
{
    (void) params;
    cli->addBuffer("Ping Marc Pong Albert and javi from munich\r\n");
}

void Server::addChannel(const std::string& channelName, const std::string& password) {
    (void) channelName;
    (void) password;
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

void Server::_handleJoin(Client *cli, std::string& params)
{
    //param format [#channel1,#channel2 key1,key2]
    std::vector<std::pair<std::string, std::string> > channelKeyPairs;    std::vector<std::string> channels, keys;
    std::istringstream iss(params);
    std::string command, channIn, keysIn;
    iss >> command;

    std::getline(iss, channIn, ' ');
    std::getline(iss, keysIn);

    if (channIn.empty() || keysIn.empty())
        throw std::invalid_argument("Invalid JOIN format, [#channel1,#channel2] [key1,key2]");
    
    channels = splitString(channIn, ',');
    keys = splitString(keysIn, ',');

    if (channels.size() != keys.size())
        throw std::invalid_argument("Number of channels does not match number of keys");
    
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].find("#") != 0)
            throw std::invalid_argument("Invalid channel format. Channels must start with '#'.");
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
    addChannel(channelName, password);
    return true;
}

void Server::disconnectClient(Client *client, std::string msg)
{
    std::string disconnect_msg = ":" + client->getNickName() + "!~" + client->getUserName() + " QUIT :" + msg + " \r\n";
    infoAllServerClients(disconnect_msg);
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
}#include "ircserv.hpp"

bool serverShutdown = false;

int main( int argc, char **argv)
{
	signal(SIGINT, signalHandler);
	signal(SIGQUIT, signalHandler);
	if (check_arguments(argc, argv) == 0) 
	{
		Server server(atoi(argv[1]));
		
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
}#include "ircserv.hpp"

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

#ifndef CHANNEL_HPP_
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
	Channel( Client *client );

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

	std::string getName();
	std::string getTopic();
	std::string getPass();

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
class Channel;

class Server
{
private:
	int _serSocketFd;
	int _port;
	std::vector<Client> _clients;
	std::vector<Channel> _channels;
	std::vector<struct pollfd> _fds;

    Client* getClient(int fd);

	void	_authenticatePassword(Client *cli, std::string& params);
    void	_setNickname(Client *cli, std::string& params);
    void	_setUser(Client *cli, std::string& params);
    void	_sendPing(Client *cli, std::string& params);
    void	_handleCapabilities(Client *cli, std::string& params);
    void	_handleQuit(Client *cli, std::string& params);
    void	_handleJoin(Client *cli, std::string& params);
    void	_handlePrivmsg(Client *cli, std::string& params);
    void	_handleKick(Client *cli, std::string& params);
    void	_handleInvite(Client *cli, std::string& params);
    void	_handleTopic(Client *cli, std::string& params);
    void	_handleMode(Client *cli, std::string& params);
	void	_handlePing(Client *cli, std::string& params);

public:
	Server();
	Server(int port);
	~Server();

	// static void signalHandler(int signum);
	void		createSocket();
	void 		loop();
	void		acceptNewClient();
	void		receiveNewData(int fd);
	void		check_comand( char *buff, Client *cli );
	void		joinChannel(Client *cli, std::string& params);

	void 		closeFds();
	void 		clearClients(int fd);
	int			getChannelIndex(); //DEBE RETORNAR EL INDICE DEL ARRAY DEL CANAL DONDE SE HA ENVIADO EL MENSAJE
	void 		addChannel(const std::string& channelName, const std::string& password);
	bool 		validateChannelPassword(Client *cli, const std::string& channelName, const std::string& password);
  	void		disconnectClient(Client *client, std::string msg);
	void		infoAllServerClients( std::string msg );
	Client		*getClientNickName( std::string NickName );
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

#endif