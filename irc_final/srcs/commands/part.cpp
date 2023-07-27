#include "../../includes/commands.hpp"

std::vector<std::string> spliit(const std::string &s, char delimiter)
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

// input[0] = part command
// input[1] = channel
// input[2] = message (optional)
int part(Server &server, Client &client, std::vector<std::string> &input)
{
    if (!client.getAuth() && !client.getRegistered())
    {
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname(), "PART"));
        return 0;
    }

    if (input.size() >= 2)
    {
	    std::vector<std::string> channels = spliit(input[1], ',');
	    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            Channel *channel = server.getChannelByName((*it).substr(1));
            if (channel)
            {
                // Check if the client is a member of this channel.
                if (!channel->isClient(&client))
                {
                    // The client is not a member of the channel.
                    client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), (*it)));
                    return 0;
                }

                // if message;
                std::string message = "";
                for (size_t i = 2; i < input.size(); i++)
                    message += input[i] + " ";

                // Broadcast the part message
                channel->broadcastMessage(PART(client.getNickname(), client.getUsername(), channel->getName(), message), NULL);
                // Client leaves the channel
                client.leaveChannel(channel);
            }
            else
            {
                    // The channel does not exist.
                    client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), (*it)));
                    return 0;

            }
        }

    }
    else
    {
        // The client didn't provide the channel name.
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "PART"));
        return 0;
    }
    return (1);
}