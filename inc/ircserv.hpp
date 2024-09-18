#ifndef IRCSERV_HPP_
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
#define ERR_OPNEEDED "482 * " + client->getNickName() + " :Operator privileges are required for this action\r\n" // El cliente no tiene permisos para canviar cosas del canal
#define ERR_PARAM461 "461 " + client->getNickName() + " :Parameters missing\r\n" // Faltan parametros al ejecutar un comando
#define ERR_TOPIC331 "331 " + params[0] + " :No channel topic set\r\n"
#define ERR_NICKN401 "401 :Invalid nickname or channel " + params[0] + " for " + params[1] + "\r\n"
#define ERR_INCHA443 "443 " + params[1] + " " + params[0] + " :User already in the channel\r\n" // Invitar un usuario que ya esta en el chanel
#define ERR_NOCHANEL "403 " + params[0] + " " + client->getNickName() + " :Channel not found\r\n" // Interactuar con un channel que no existe
#define MSG_TOPIC332 "332 " + params[0] + " :Current topic is: " + channel->getTopic() + "\r\n"
#define ERR_UNKCMD421 "421 " + client->getNickName() + " :Unknown command or invalid parameters\r\n"

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