#ifndef COMMANDS_HPP

# define COMMANDS_HPP

# include "Client.hpp"
# include "Server.hpp"
# include "Channel.hpp"
# include "define.hpp"
# include <vector>

class Server;
class Client;
class Channel;

// add all the commands here

int cap(Server &server, Client &client, std::vector<std::string> &input);
int die(Server &server, Client &client, std::vector<std::string> &input);
int info(Server &server, Client &client, std::vector<std::string> &input);
int invite(Server &server, Client &client, std::vector<std::string> &input);
int join(Server &server, Client &client, std::vector<std::string> &input);
int kick(Server &server, Client &client, std::vector<std::string> &input);
int mode(Server &server, Client &client, std::vector<std::string> &input);
int names(Server &server, Client &client, std::vector<std::string> &input);
int names(Server &server, Client &client, std::vector<std::string> &input);
int nick(Server &server, Client &client, std::vector<std::string> &input);
int notice(Server &server, Client &client, std::vector<std::string> &input);
int oper(Server &server, Client &client, std::vector<std::string> &input);
int part(Server &server, Client &client, std::vector<std::string> &input);
int pass(Server &server, Client &client, std::vector<std::string> &input);
int ping(Server &server, Client &client, std::vector<std::string> &input);
int privmsg(Server &server, Client &client, std::vector<std::string> &input);
int quit(Server &server, Client &client, std::vector<std::string> &input);
int topic(Server &server, Client &client, std::vector<std::string> &input);
int user(Server &server, Client &client, std::vector<std::string> &input);
int wallops(Server &server, Client &client, std::vector<std::string> &input);
int whois(Server &server, Client &client, std::vector<std::string> &input);


#endif