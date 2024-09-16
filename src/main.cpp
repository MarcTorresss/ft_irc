#include "ircserv.hpp"

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
		catch(...)
		{
			//std::cout << "Exception caught" << std::endl;
		}
	}
	else
	{
		std::cout << "Usage is ./ircserv [port] [password]" << std::endl;
		return -1;
	}
}