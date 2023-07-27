#include "../../includes/commands.hpp"

int modeClient(Server &server, Client &client, std::vector<std::string> &input)
{
    (void)server;
    std::string msg = "";

    bool add = true;
    for (size_t i = 0; i < input[2].size(); i++)
    {
        if (input[2][i] == '+')
            add = true;
        else if (input[2][i] == '-')
            add = false;
        else
        {
            if (input[2][i] == 'i')
                client.setInvisible(add);
            else if (input[2][i] == 'w')
                client.setWallops(add);
            else
            {
                client.sendMessage(ERR_UMODEUNKNOWNFLAG(client.getNickname()));
                continue ;
            }
            if (add)
                msg += "+";
            else
                msg += "-";
            msg += input[2][i];
        }
    }

    client.sendMessage(MODE_USER(client.getNickname(), client.getUsername(), client.getNickname(), msg));

    return (1);
}

int modeChannel(Server &server, Client &client, std::vector<std::string> &input, Channel *channel)
{
    (void)server;
    std::string msg = "";

    Client *op = NULL;

    bool add = true;
    for (size_t i = 0; i < input[2].size(); i++)
    {
        if (input[2][i] == '+')
            add = true;
        else if (input[2][i] == '-')
            add = false;
        else
        {
            if (input[2][i] == 'i')
            {
                // Set or remove Invite-only channel
                channel->setInviteOnly(add);
            }
            else if (input[2][i] == 't')
            {
                // Set or remove the restrictions of the TOPIC command to channel operators
                channel->setTopicRestricted(add);
            }
            else if (input[2][i] == 'k')
            {
                // Set or remove the channel key (password)
                if (add)
                {
                    if (input.size() < 4)
                    {
                        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
                        continue ;
                    }
                    channel->setPassword(input[3]);
                }
                else
                    channel->setPassword("");
            }
            else if (input[2][i] == 'o')
            {
                // Give/take channel operator privilege
                if (input.size() < 4)
                {
                    client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
                    continue ;
                }
                op = channel->getClientByNickname(input[3]);
                if (!op)
                {
                    client.sendMessage(ERR_USERNOTINCHANNEL(client.getNickname(), input[1].substr(1), input[3]));
                    continue ;
                }
                if (add)
                    channel->addOperator(op);
                else
                    channel->removeOperator(op);
            }
            else if (input[2][i] == 'l')
            {
                // Set/remove the user limit to channel
                if (add)
                {
                    if (input.size() < 4)
                    {
                        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
                        continue ;
                    }
                    int limit = std::atoi(input[3].c_str());  // Use atoi instead of stoi
                    channel->setLimit(limit);
                }
                else
                    channel->setLimit(0);
            }
            else
            {
                client.sendMessage(ERR_UMODEUNKNOWNFLAG(client.getNickname()));
                continue ;
            }
			if (add)
				msg += "+";
			else
				msg += "-";
            msg += input[2][i];
        }
    }

    client.sendMessage(MODE_CHANNEL(client.getNickname(), client.getUsername(), input[1].substr(1), msg));

    return 1;
}

// input[0] = mode command
// input[1] = channel name
// input[2] = -i +i etc.
// input[3] = password / nickname / limit (optional)
int mode(Server &server, Client &client, std::vector<std::string> &input)
{
    if (!client.getAuth() && !client.getRegistered())
    {
        // Client already authenticate
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname(), "MODE"));
        return 0;
    }
    if (input.size() < 3)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
        return 0;
    }

    // mode channel
    if (input[1][0] == '#')
    {
        Channel *channel = server.getChannelByName(input[1].substr(1));

        if (!channel)
        {
            client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), input[1]));
            return 0;
        }
        if (!channel->isOperator(&client))
        {
            // The client is not an operator in the specified channel.
            client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), input[1]));
            return (0);
        }

        return (modeChannel(server, client, input, channel));
    }
    else if (input[1] == client.getNickname())
        return (modeClient(server, client, input));
    else
        client.sendMessage(ERR_USERSDONTMATCH(client.getNickname()));
    return (0);
}