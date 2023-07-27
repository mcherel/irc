#ifndef CHANNEL_HPP

# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
# include <algorithm>

# include "Client.hpp"
# include "Server.hpp"

class Client;
class Server;

class Channel {
    private:
		Server					&_server;
		Client 					*_creator;
        std::string				_name;
		std::string				_topic;
        std::string				_password;
        std::vector<Client *>	_clients;
        std::vector<Client *>	_operators;
        std::vector<Client *>	_invited;
		size_t					_limit;
		bool					_inviteOnly;
		bool					_topicRestricted;

    public:
		Channel(Server &server, Client *client, std::string name);
        ~Channel();

		void	broadcastMessage(const std::string &message, Client *client);

		// Getters

		Client 					*getCreator() { return _creator; };
		std::string 			getName() { return _name; };
		std::string 			getTopic() { return _topic; };
		std::string 			getPassword() { return _password; };
		std::vector<Client *>	getClients() { return _clients; };
		std::vector<Client *>	getOperators() { return _operators; };
		std::vector<Client *>	getInvited() { return _invited; };
		size_t 					getLimit() { return _limit; };
		bool 					getInviteOnly() { return _inviteOnly; };
		bool 					getTopicRestricted() { return _topicRestricted; };

		// Setters
		
		void 					setCreator(Client *creator) { _creator = creator; };
		void 					setName(std::string name) { _name = name; };
		void 					setTopic(std::string topic) { _topic = topic; };
		void 					setPassword(std::string password) { _password = password; };
		void 					setLimit(size_t limit) { _limit = limit; };
		void 					setInviteOnly(bool inviteOnly) { _inviteOnly = inviteOnly; } ;
		void 					setTopicRestricted(bool topicRestricted) { _topicRestricted = topicRestricted; } ;

		int						isCreator(Client *client) { return _creator == client; };

		void 					addClient(Client *client);
		void 					removeClient(Client *client);
		int 					isClient(Client *client);
		Client 					*getClientByNickname(std::string &nickname);

		void 					addOperator(Client *client);
		void 					removeOperator(Client *client);
		int 					isOperator(Client *client);

		void 					addInvited(Client *client);
		void 					removeInvited(Client *client);
		int 					isInvited(Client *client);
};

#endif