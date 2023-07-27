#include "../../includes/commands.hpp"

// Command to quit the irc
// input[0] = quit command
// input[1] = message why quit (optional)
int quit(Server &server, Client &client, std::vector<std::string> &input)
{
	std::vector<Channel *> channels = client.getChannels();

    if (input.size() < 2)
    {
        // Not enough parameters were provided.
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), input[0]));
        return 0;
    }

    // Notify other clients in the channels that this client is part of.
    for (std::vector<Channel *>::iterator channel = channels.begin(); channel != channels.end(); channel++)
        (*channel)->broadcastMessage(QUIT(client.getNickname(), client.getUsername(), input[1].substr(1)), &client);

    // Disconnect the client.
    server.removeClient(&client);

    return 1;
}