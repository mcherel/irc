#include "../includes/Channel.hpp"

Channel::Channel(Server &server, Client *client, std::string name) : 
	_server(server),
	_creator(client),
	_name(name),
	_limit(0),
	_inviteOnly(false),
	_topicRestricted(false)
	{
		this->addOperator(client);
	}

Channel::~Channel() {
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		(*it)->leaveChannel(this);
}

void	Channel::broadcastMessage(const std::string &message, Client *client) {
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		if ((*it) != client)
			(*it)->sendMessage(message);
}

// --------------------CLIENT--------------------

void Channel::addClient(Client *client)
{
	if (!client)
		return ;
    if (_limit > 0 && _clients.size() >= _limit)
    {
        client->sendMessage("LIMIT MSG CHANNEL FULL");
        return ;
    }
	std::vector<Client *>::iterator it = std::find(_clients.begin(), _clients.end(), client);
	if (it != _clients.end())
		return ;
    _clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
	if (!client)
		return ;
	std::vector<Client*>::iterator it = std::find(_clients.begin(), _clients.end(), client);
    if (it == _clients.end())
        return ;
    _clients.erase(it);
	if (client == _creator)
		_creator = NULL;
	removeInvited(client);
	removeOperator(client);
	if (_operators.size() == 0 && _clients.size() > 0)
		addOperator(*_clients.begin());
	if (_clients.size() == 0 && _operators.size() == 0)
		_server.removeChannel(this);
}

Client *Channel::getClientByNickname(std::string &nickname)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        if ((*it)->getNickname() == nickname)
			return (*it);
    return (0);
}

int Channel::isClient(Client *client)
{
	if (!client)
		return (0);
	std::vector<Client*>::iterator it = std::find(_clients.begin(), _clients.end(), client);
    if (it != _clients.end())
        return (1);
    return (0);
}

// --------------------OPERATOR--------------------

void Channel::addOperator(Client *client)
{
	if (!client)
		return ;
	std::vector<Client*>::iterator it = std::find(_operators.begin(), _operators.end(), client);
	if (it != _operators.end())
		return ;
    _operators.push_back(client);
}

void Channel::removeOperator(Client *client)
{
	if (!client)
		return ;
	std::vector<Client*>::iterator it = std::find(_operators.begin(), _operators.end(), client);
    if (it == _operators.end())
        return ;
    _operators.erase(it);
}

int Channel::isOperator(Client *client)
{
	if (!client)
		return (0);
	std::vector<Client*>::iterator it = std::find(_operators.begin(), _operators.end(), client);
    if (it != _operators.end())
        return (1);
    return (0);
}

// --------------------INVITED--------------------

void Channel::addInvited(Client *client)
{
	if (!client)
		return ;
	std::vector<Client*>::iterator it = std::find(_invited.begin(), _invited.end(), client);
	if (it != _invited.end())
		return ;
    _invited.push_back(client);
}

void Channel::removeInvited(Client *client)
{
	if (!client)
		return ;
	std::vector<Client*>::iterator it = std::find(_invited.begin(), _invited.end(), client);
    if (it == _invited.end())
        return ;
    _invited.erase(it);
}

int Channel::isInvited(Client *client)
{
	if (!client)
		return (0);
	std::vector<Client*>::iterator it = std::find(_invited.begin(), _invited.end(), client);
    if (it != _invited.end())
        return (1);
    return (0);
}