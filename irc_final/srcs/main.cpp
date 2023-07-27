#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include <csignal>

// While it's false, the server can run
// If it turns true, the server stop running
bool	g_shutdown = false;

static void	shutdown(int)
{
	g_shutdown = true;
}

int	main(int ac, char **av)
{
	// If we ctrl+c, we quit clean
	signal(SIGINT, shutdown);
	signal(SIGPIPE, SIG_IGN);
	if (ac != 3)
		return (std::cerr << "Error: Usage: ./ircserv <port> <password>" << std::endl, 0);

	// Convert av[1] to an int (port) 
	int port = std::atoi(av[1]);
	// Take av[2] (password)
	std::string password = av[2];

	// Create a server and run it
	try {
		Server irc(port, password);
	}
	// If a problem happened, print the error
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return (0);
}