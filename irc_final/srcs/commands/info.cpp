#include "../../includes/commands.hpp"

// INFO command
// input[0] = INFO command
// input[1] = server name
int info(Server &server, Client &client, std::vector<std::string> &input)
{
    (void)server;
    // Check if the necessary arguments are provided
    if (input.size() >= 2 && input[1] != SERVER_NAME)
    {
        client.sendMessage(ERR_NOSUCHSERVER(client.getNickname()));
        return 0;
    }

    // Send the INFO response
    client.sendMessage(RPL_INFO(client.getNickname()));
    client.sendMessage(RPL_ENDOFINFO(client.getNickname()));

    return 1;
}