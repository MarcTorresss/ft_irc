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
#include <cstdlib>

//-------------------------COLORS------------------------------//
#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

#define ERR "\e[1;31mError: "

//---------------------------CLASSES---------------------------//
#include "Client.hpp"
#include "Server.hpp"

#endif