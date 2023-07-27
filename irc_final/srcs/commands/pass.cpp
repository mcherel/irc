#include "../../includes/commands.hpp"

// input[0] = command
// input[1] = password
// NICK UNITIALISED HERE BC PASS IS FIRST COMMAND
int pass(Server &server, Client &client, std::vector<std::string> &input)
{
    if (input.size() < 2)
    {
        // Not enough parameters were provided.
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), input[0]));
        return 0;
    }

    if (client.getRegistered())
    {
        // Client already authenticate
        client.sendMessage(ERR_ALREADYREGISTRED(client.getNickname()));
        return 0;
    }

    if (server.getPassword() == input[1])
        client.setAuth(true);
    else
    {
        // Not enough parameters were provided.
        client.sendMessage(ERR_PASSWDMISMATCH(client.getNickname()));
        return 0;
    }

    return 1;
}