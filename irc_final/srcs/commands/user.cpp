#include "../../includes/commands.hpp"

// Command to change username
// input[0] = USER command
// input[1] = new username
// input[2] = mode
// input[3] = unused
// input[4] = realname
int user(Server &server, Client &client, std::vector<std::string> &input)
{
    (void)server;
    // Check if the necessary arguments are provided
    if (input.size() < 5)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), input[0]));
        return 0;
    }

    if (client.getRegistered())
    {
        // Client already registered
        client.sendMessage(ERR_ALREADYREGISTRED(client.getNickname()));
        return 0;
	}

    std::string newUsername = input[1];

    // Set Realname
    if (input[4][0] == ':')
        input[4] = input[4].substr(1);
    client.setRealname(input[4]);

    // Update the client's username
    client.setUsername(newUsername);

    if (!client.getNickname().empty() && !client.getUsername().empty() && !client.getHostname().empty())
    {
        client.setRegistered(true);
    }

	if (client.getRegistered() && client.getAuth())
        client.sendMessage(RPL_WELCOME(client.getNickname()));

    return 1;
}