#include "../../includes/commands.hpp"

int wallops(Server &server, Client &client, std::vector<std::string> &input)
{
    if (!client.getAuth() && !client.getRegistered())
    {
        // Client already authenticate
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname(), input[0]));
        return 0;
    }
    // Check if the necessary arguments are provided
    if (input.size() < 2)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), input[0]));
        return 0;
    }

    // Check if the client is an operator
    if (!client.getOp())
    {
        client.sendMessage(ERR_NOPRIVILEGES(client.getNickname()));
        return 0;
    }

    // Prepare the message to send
    std::string message = "";
    for (int i = 1; i < (int)input.size(); i++)
        message += input[i] + " ";
    if (message[0] == ':')
        message = message.substr(1);

    // Send the message to all operators
    std::vector<Client *> clients = server.getClients();
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
        if ((*it)->getOp() || (*it)->getWallops())
            (*it)->sendMessage(":" + client.getNickname() + " WALLOPS :" + message + "\r\n");

    return 1;
}