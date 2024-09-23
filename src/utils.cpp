#include "ircserv.hpp"
#include "server.hpp"

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

std::vector<std::string> splitString(const std::string& input, char delimiter)
{
    std::istringstream stream(input);
    std::string token;
    std::vector<std::string> ret;

    while (std::getline(stream, token, delimiter))
        ret.push_back(token);
    
    return ret;
}