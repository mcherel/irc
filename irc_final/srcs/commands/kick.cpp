#include "../../includes/commands.hpp"

// input[0] = kick command
// input[1] = Channel name
// input[2] = nickname of user kicked
// input[3] = message / reason for the kick (optional)
int kick(Server &server, Client &client, std::vector<std::string> &input)
{
    if (!client.getAuth() && !client.getRegistered())
    {
        // Client already authenticate
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname(), "KICK"));
        return 0;
    }
    // Check that we have at least channel name and nickname of user
    if (input.size() < 3)
    {
        // The client didn't provide enough arguments.
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
        return (0);
    }

    std::string channelName = input[1].substr(1);
    Channel *channel = server.getChannelByName(channelName);
    if (!channel)
    {
        // The channel doesn't exist.
        client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), input[1]));
        return (0);
    }

    if (!client.getChannel(channelName))
    {
        client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), input[1]));
        return (0);
    }

    // Check if client is an operator in the channel.
    if (!channel->isOperator(&client))
    {
        // The client is not an operator in the specified channel.
        client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel->getName()));
        return (0);
    }

    Client *clientToKick = channel->getClientByNickname(input[2]);
    if (!clientToKick)
    {
        // The user to kick doesn't exist.
        client.sendMessage(ERR_USERNOTINCHANNEL(client.getNickname(), channel->getName(), input[2]));
        return (0);
    }


    std::string kickMessage = "";
    for (size_t i = 3; i < input.size(); i++)
        kickMessage += input[i] + " ";
    channel->broadcastMessage(KICK(client.getNickname(), client.getUsername(), channel->getName(), clientToKick->getNickname(), kickMessage), NULL);
    // Kick the user from the channel.
    clientToKick->leaveChannel(channel);

    return (1);
}