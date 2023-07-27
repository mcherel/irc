#include "../../includes/commands.hpp"

// input[0] = invite command
// input[1] = nickname of user to invite
// input[2] = channel name
int invite(Server &server, Client &client, std::vector<std::string> &input)
{
    if (!client.getAuth() && !client.getRegistered())
    {
        // Client already authenticate
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname(), "INVITE"));
        return 0;
    }
    if (input.size() < 3)
    {
        // The client didn't provide enough arguments.
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
        return (0);
    }

    std::string channelName = input[2];
    if (channelName[0] == '#')
        channelName = input[2].substr(1);

    Channel *channel = server.getChannelByName(channelName);
    if (!channel)
    {
        // The channel doesn't exist.
        client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return (0);
    }

    // Check if client is part of the channel.
    if (!channel->isClient(&client))
    {
        // The client is not part of the specified channel.
        client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), channelName));
        return (0);
    }

    Client *clientToInvite = server.getClientByNickname(input[1]);
    if (!clientToInvite)
    {
        // The user to invite doesn't exist.
        client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), input[1]));
        return (0);
    }

    // Check if client to invite is already part of the channel.
    if (channel->isClient(clientToInvite))
    {
        client.sendMessage(ERR_USERONCHANNEL(client.getNickname(), channelName, input[1]));
        return (0);
    }

	client.sendMessage(RPL_INVITING(client.getNickname(), clientToInvite->getNickname(), channelName));
    clientToInvite->sendMessage(INVITE(client.getNickname(), client.getUsername(), clientToInvite->getNickname(), channelName));
    
    // Invite the user to the channel.
    channel->addInvited(clientToInvite);

    return (1);
}