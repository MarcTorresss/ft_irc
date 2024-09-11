#include "ircserv.hpp"

bool serverShutdown = false;

int main( int argc, char **argv)
{
	signal(SIGINT, signalHandler);
	if (check_arguments(argc, argv) < 0)
		return -1;
	Server server(atoi(argv[1]));
}