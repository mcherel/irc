#include "../../includes/commands.hpp"

extern bool g_shutdown;

// turn off the server
int die(Server &server, Client &client, std::vector<std::string> &input)
{
    (void)server;
    (void)input;
    
    if (!client.getAuth() && !client.getRegistered())
    {
        // Client already authenticate
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname(), "DIE"));
        return 0;
    }

    // Only allow IRC operators to use this command.
    if (!client.getOp()) 
    {
        client.sendMessage(ERR_NOPRIVILEGES(client.getNickname()));
        return 0;
    }

    g_shutdown = true;

    return 1;
}
