#include "Server.hpp"

bool Server::_signal = false;

Server::Server(): _serSocketFd(-1), _port(4444){
}

Server::Server(int port): _serSocketFd(-1), _port(port){
}

Server::~Server(){

}

void Server::serverInit()
{
	serSocket();

	std::cout << GRE << "Server <" << _serSocketFd << "> Connected" << WHI << std::endl << "";
	std::cout << "Server is ready, waiting for clients..." << std::endl;

	while (!Server::_signal)
	{
		int pollResult = poll(&_fds[0],_fds.size(),-1);
    	if (pollResult == -1 && !Server::_signal)
		{
        	throw std::runtime_error("Thee function poll() failed");
		}
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _serSocketFd)
					acceptNewClient();
				else
					receiveNewData(_fds[i].fd);
			}
		}
	}
	//closeFds();
}

void Server::signalHandler(int signum)
{
	(void)signum;
	Server::_signal = true;
}

void Server::serSocket()
{
	struct	sockaddr_in sockAddr;
	struct	pollfd		serPoll;
	// struct pollfd poll_a; //;)

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(_port);
	sockAddr.sin_addr.s_addr = INADDR_ANY;
	std::memset(sockAddr.sin_zero, 0, sizeof(sockAddr.sin_zero));

	_serSocketFd = socket(AF_INET, SOCK_STREAM, 0); //comunicacion ipv4 por tcp, protocolo variable segun el sistema
	if (_serSocketFd == -1)
		throw(std::logic_error("faild to create socket"));
	int en = 1;
	if(setsockopt(_serSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) // configuramos el socket con SO_REUSEADDR para poder reutilizar puertos o ips
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	if (fcntl(_serSocketFd, F_SETFL, O_NONBLOCK) == -1) // funciones como read(), write(), accept(), no se quedaran colgadas volveran con un error
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(_serSocketFd, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == -1) // asignamos una ip y un puerto
		throw(std::runtime_error("faild to bind socket"));
	if (listen(_serSocketFd, SOMAXCONN) == -1) // vinculamos el fd y configuramos la cantidad maxima de solicitudes de conexion
		throw(std::runtime_error("listen() faild"));

	serPoll.fd = _serSocketFd; //el port d'escolta del serversocket
	serPoll.events = POLLIN;  // poll() nos dira si hay datos para leer en fd o nuevas conexiones en un socket de servidor 
	_fds.push_back(serPoll); //afagir el pollfd al vector
}

void Server::acceptNewClient()
{
	/*Client client;
	struct sockaddr_in cliAddr;
	socklen_t len = sizeof(cliAddr);
	//struct pollfd cliPoll;
	int cliSocket = accept(_serSocketFd, (struct sockaddr *)&(cliAddr), &len);
	if (cliSocket < 0) {
		std::cout << "unable to accept new client" << std::endl;
		return;
	}*/
	//Temporal es solo para que funcione falta lo de javi, arriba in processss
	std::cout << GRE << "NEW CLIENT" << WHI << std::endl;
	Client cli; //-> create a new client
	struct sockaddr_in cliadd;
	struct pollfd NewPoll;
	socklen_t len = sizeof(cliadd);

	int incofd = accept(_serSocketFd, (sockaddr *)&(cliadd), &len); //-> accept the new client
	if (incofd == -1)
		{std::cout << "accept() failed" << std::endl; return;}

	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		{std::cout << "fcntl() failed" << std::endl; return;}

	NewPoll.fd = incofd; //-> add the client socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0; //-> set the revents to 0

	cli.setFd(incofd); //-> set the client file descriptor
	cli.setIpAdd(inet_ntoa((cliadd.sin_addr))); //-> convert the ip address to string and set it
	_clients.push_back(cli); //-> add the client to the vector of clients
	_fds.push_back(NewPoll); //-> add the client socket to the pollfd

	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

Client* Server::getClient(int fd) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getFd() == fd) {
            return &(*it);
        }
    }
    return NULL;
}

void	Server::check_comand( char *buff, Client *cli )
{
	std::string receivedData(buff);
    std::istringstream iss(receivedData);
    std::string command;
    std::string params;

    // Separar el comando de los parámetros
    iss >> command;
    std::getline(iss, params);

    std::string commands[] = { "NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE", "PASS" };
    int i = 0;
    for (i = 0; i < 9; i++) {
        if (commands[i] == command)
            break;
    }
	(void) cli;
    switch (i)
    {
        case 0: // NICK
            //_setNickname(cli, params);
			std::cout << "entro" << std::endl;
            break;
        case 1: // USER
            //_setUser(cli, params);
            break;
        case 2: // JOIN
            //_handleJoin(cli, params);
            break;
        case 3: // PRIVMSG
            //_handlePrivmsg(cli, params);
            break;
        case 4: // KICK
            //_handleKick(cli, params);
            break;
        case 5: // INVITE
            //_handleInvite(cli, params);
            break;
        case 6: // TOPIC
            //_handleTopic(cli, params);
            break;
        case 7: // MODE
            //_handleMode(cli, params);
            break;
        case 8: // PASS
            //_authenticatePassword(cli, params);
            break;
        default:
            std::cout << "Comando no reconocido: " << command << std::endl;
            break;
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

void Server::clearClients(int fd){
	for(size_t i = 0; i < _fds.size(); i++){ //-> remove the client from the pollfd
		if (_fds[i].fd == fd)
			{_fds.erase(_fds.begin() + i); break;}
	}
	for(size_t i = 0; i < _clients.size(); i++){ //-> remove the client from the vector of clients
		if (_clients[i].getFd() == fd)
			{_clients.erase(_clients.begin() + i); break;}
	}
	close(fd); // Tanquem definitivament el fd
}