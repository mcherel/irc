#include "../../includes/commands.hpp"

// Command to change nickname
// input[0] = nick command
// input[1] = new nickname
// NICKNAME de .getNickname() UNITIALISED HERE BECAUSE NICK SECOND COMMAND
int nick(Server &server, Client &client, std::vector<std::string> &input)
{
    // Check if the necessary arguments are provided
    if (input.size() < 2)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "NICK"));
        return 0;
    }

    std::string newNickname = input[1];

    // If user try to change nickname to one that already exist
    if (server.getClientByNickname(newNickname) && !client.getNickname().empty())
    {
			client.sendMessage(ERR_NICKNAMEINUSE(newNickname));
			return 0;
    }

    // Otherwise, it's a new user. Check if the new nickname is already in use
    while (server.getClientByNickname(newNickname))
        newNickname += "_";

    // Update the client's nickname
    client.setNickname(newNickname);

    // Notify the client that the nickname change was successful
    client.sendMessage(NICK(client.getNickname(), client.getHostname(), newNickname));

    if (!client.getNickname().empty() && !client.getUsername().empty() && !client.getHostname().empty())
        client.setRegistered(true);
    if (client.getRegistered() && client.getAuth())
        client.sendMessage(RPL_WELCOME(client.getNickname()));

    return 1;
}