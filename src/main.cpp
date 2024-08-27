#include "ircserv.hpp"

int check_arguments(int argc, char **argv){
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
	if (portnum > 65535){
		std::cerr << ERR <<"Port number too big (max is 65535)" << WHI<< std::endl;
		return -1;
	}
	std::string password(argv[2]);
	if (password.length() > 30){ //numero arbitrario cambiable 
		std::cerr << ERR <<"Password is too long (max 30 chars)" << WHI<< std::endl;
		return -1;
	}
	return 1;
}


int main( int argc, char **argv)
{
	if (check_arguments(argc, argv) < 0)
		return -1;
	Server server(atoi(argv[1]));
}