#include "ircserv.hpp"

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
		check_comand(buff, cli);
    }
}

void	Server::check_comand( char *buff, Client *cli )
{
	std::string receivedData(buff);
    std::istringstream iss(receivedData);
    std::string command;
    std::string space;
	std::vector<std::string> params;
	std::string temp = "";

	if (receivedData.find(std::string("\r\n")) != std::string::npos
		|| receivedData.find(std::string("\n")) != std::string::npos)
	{
		// Este bucle procesa las líneas de mensajes desde el cliente.
		while (!receivedData.empty())
		{
			std::cout << "The data:" << receivedData << "." << std::endl;
			// Buscar el delimitador de fin de línea
			size_t pos = receivedData.find("\r\n");
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
}