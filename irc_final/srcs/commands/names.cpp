#include "../../includes/commands.hpp"

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

// sans input[1], montre tous les nicks du serveur
// input[0] = names command
// input[...] = channel (optional)
int names(Server &server, Client &client, std::vector<std::string> &input)
{
    if (input.size() >= 2)
    {
		std::vector<std::string> channelNames = split(input[1], ',');
		for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); it++)
        {
			Channel	*channel = server.getChannelByName((*it).substr(1));
            if (channel)
            {
                // Show all the members of that channel
                std::vector<Client *> clients = channel->getClients();
                std::string clientsName;

                for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
                {
                    if ((*it) == channel->getCreator())
                        clientsName += "@";
                    clientsName += (*it)->getNickname() + " ";
                }
			    client.sendMessage(RPL_NAMREPLY(client.getNickname(), *it, clientsName));
			    client.sendMessage(RPL_ENDOFNAMES(client.getNickname(), *it));
            }
            else
                client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), *it));
        }
    }
    else
    {
        std::vector<Client *> clients = server.getClients();
        std::string clientsName;
        for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
            clientsName += (*it)->getNickname() + " ";

        client.sendMessage(clientsName);
    }

    return 1;
}