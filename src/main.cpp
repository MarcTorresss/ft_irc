#include "ircserv.hpp"

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
}