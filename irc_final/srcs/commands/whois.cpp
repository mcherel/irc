#include "../../includes/commands.hpp"

// WHOIS command
// input[0] = WHOIS command
// input[1] = nickname of the user to get information about
int whois(Server &server, Client &client, std::vector<std::string> &input)
{
    (void)server;
    (void)client;
    (void)input;
    return (1);
    // // Check if the necessary arguments are provided
    // if (input.size() < 2)
    // {
    //     client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), input[0]));
    //     return 0;
    // }

    // // Get the target client
    // Client* target = server.getClientByNickname(input[1]);
    // if (!target)
    // {
    //     client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), input[1]));
    //     return 0;
    // }

    // // Send the WHOIS response
    // client.sendMessage(RPL_WHOISUSER(target->getNickname(), target->getUsername(), target->getHostname(), target->getRealname()));
    // client.sendMessage(RPL_WHOISSERVER(target->getNickname(), server.getName(), server.getInfo()));
    // client.sendMessage(RPL_ENDOFWHOIS(target->getNickname()));

    // return 1;
}