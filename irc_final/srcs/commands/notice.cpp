#include "../../includes/commands.hpp"

// input[0] = notice command
// input[1] = receiver (client / channel)
// input[...] = message to send
int notice(Server &server, Client &client, std::vector<std::string> &input)
{
    if (!client.getAuth() && !client.getRegistered())
    {
        // Client already authenticate
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname(), "NOTICE"));
        return 0;
    }
    if (input.size() < 3)
    {
        // Not enough parameters were provided.
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), input[0]));
        return 0;
    }

    std::string receiver = input[1];
    std::string message;
    for (int i = 2; i < (int)input.size(); i++)
        message += input[i] + " ";

    if (message[0] == ':')
        message = message.substr(1);

    // Check if the receiver is a channel.
    if (receiver[0] == '#')
    {
        Channel *channel = server.getChannelByName(receiver.substr(1));
        if (channel)
        {
            // The receiver is a channel.
            // Make sure the client is a member of this channel.
            if (!channel->isClient(&client))
            {
                // The client is not a member of the channel.
                client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), receiver));
                return 0;
            }

            // Send the notice to all members of the channel.
            channel->broadcastMessage(NOTICE(client.getNickname(), client.getHostname(), receiver, message), &client);
            return 1;
        }
        else
        {
            client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), receiver));
            return (0);
        }
    }

    // If the receiver is not a channel, assume it is a client.
    Client *receiverClient = server.getClientByNickname(receiver);
    if (receiverClient)
    {
        // Send the notice to the other client.
        receiverClient->sendMessage(NOTICE(client.getNickname(), client.getHostname(), receiver, message));
        return 1;
    }

    // The receiver does not exist.
    client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), receiver));
    return 0;
}