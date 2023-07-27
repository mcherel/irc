#include "../../includes/commands.hpp"

// input[0] = join command
// input[1] = channel
// input[2] = password (optional)
int join(Server &server, Client &client, std::vector<std::string> &input)
{
    if (!client.getAuth() && !client.getRegistered())
    {
        // Client already authenticate
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname(), "JOIN"));
        return 0;
    }
    // Check that we have at least one argument (channel)
    if (input.size() < 2)
    {
        // The client didn't provide a channel name to join.
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), input[0]));
        return (0);
    }

    std::string channelName = input[1].substr(1);
    Channel *channel = server.getChannelByName(channelName);
    // If the channel doesn't exist, create a new one.
    if (!channel)
    {
        channel = new Channel(server, &client, channelName);
        server.addChannel(channel);
    }
    // Else, check password, mode, etc.
    else
    {
        if (!channel->getPassword().empty())
        {
            if (input.size() < 3 || channel->getPassword() != input[2])
            {
                // Password is missing or wrong.
                client.sendMessage(ERR_BADCHANNELKEY(client.getNickname(), channel->getName()));
                return (0);
            }
        }
        if (channel->getInviteOnly() && !channel->isInvited(&client))
        {
            // Client is not on the invite list.
            client.sendMessage(ERR_INVITEONLYCHAN(client.getNickname(), channel->getName()));
            return (0);
        }
        if (channel->getLimit() && channel->getClients().size() >= channel->getLimit())
        {
            // Client is not on the invite list.
            client.sendMessage(ERR_CHANNELISFULL(client.getNickname(), channel->getName()));
            return (0);
        }
    }
    // Add the client to the channel.
    client.joinChannel(channel);
    // Send a message to the channel that the client has joined.
    channel->broadcastMessage(JOIN(client.getNickname(), client.getUsername(), channel->getName()), NULL);
    channel->removeInvited(&client);
    // topic(server, client, input);
    names(server, client, input);
    return (1);
}