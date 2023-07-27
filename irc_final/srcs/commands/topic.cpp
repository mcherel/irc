#include "../../includes/commands.hpp"

// Command to setup a topic for the channel
// input[0] = topic
// input[1] = channel
// input[2] = message
int topic(Server &server, Client &client, std::vector<std::string> &input)
{
    if (!client.getAuth() && !client.getRegistered())
    {
        // Client already authenticate
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname(), "TOPIC"));
        return 0;
    }
    if (input.size() < 2)
	{
        // The client didn't provide enough arguments.
        // Send a message back to the client to let them know.
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
        return (0);
    }

    std::string channelName = input[1].substr(1);
    Channel *channel = server.getChannelByName(channelName);
    if (!channel)
	{
        // The specified channel does not exist.
        // Send a message back to the client to let them know.
        client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return (0);
    }

    // Check if client is part of the channel.
    if (!channel->isClient(&client))
    {
        // The client is not part of the specified channel.
        // Send a message back to the client to let them know.
        client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), channelName));
        return (0);
    }
    
    // If no topic was provided, return the current topic.
    if (input.size() == 2)
    {
        std::string currentTopic = channel->getTopic();
        if (currentTopic.empty())
            client.sendMessage(RPL_NOTOPIC(client.getNickname(), channel->getName()));
		else
			client.sendMessage(RPL_TOPIC(client.getNickname(), channel->getName(), currentTopic));
        return 1;
    }
    else // A new topic was provided. Set the new topic.
    {
        std::string topic = "";
        for (int i = 2; i < (int)input.size(); i++)
            topic += input[i] + " ";

        // if topicRestricted, then Only operator can change topic.
        if (channel->getTopicRestricted() && !channel->isOperator(&client))
        {
            client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel->getName()));
            return (0);
        }
        
	    if (topic[0] == ':')
		    topic = topic.substr(1);
        // Set the new topic.
        channel->setTopic(topic);

        // Notify the client that the topic was successfully changed.
        channel->broadcastMessage(TOPIC(client.getNickname(), client.getUsername(), channel->getName(), channel->getTopic()), NULL);
	}
	return (1);
}