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
	std::cout << WHI << "PORT IS: " << port << RED << std::endl;
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
	if (password.length() > 30){ //numero arbitrario cambiable 
		std::cerr << ERR <<"Password is too long (max 30 chars)" << WHI<< std::endl;
		return -1;
	}
	return 1;
}