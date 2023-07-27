#ifndef CLIENT_HPP

# define CLIENT_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <algorithm>

# include "Channel.hpp"
# include "Server.hpp"

class Channel;
class Server;

class Client {
    private:
		int						_fd;
        std::string				_hostname; // Name of the computer client is connecting from
        std::string				_username; // Name used for authentication purposes (unique)
        std::string				_nickname; // Name used to identify user in chat
		std::string				_realname;
        std::string				_partialInput;
		std::vector<Channel *>	_channels;
		bool					_isRegistered;
		bool					_isAuth;
		bool					_isOp;
		bool					_isInvisible;
		bool					_isWallops;

    public:
        Client(int fd, const std::string name);
        ~Client();

		// Getters
		int			getFd() { return _fd; };
		std::string getHostname() { return _hostname; };
		std::string getUsername() { return _username; };
		std::string getNickname() { return _nickname; };
		std::string getRealname() { return _realname; };
		std::string getPartialInput() { return _partialInput; };
		std::vector<Channel *> getChannels() { return _channels; };
		bool 		getRegistered() { return _isRegistered; };
		bool 		getAuth() { return _isAuth; };
		bool 		getOp() { return _isOp; };
		bool 		getInvisible() { return _isInvisible; };
		bool 		getWallops() { return _isWallops; };


		// Setters
		void 		setFd(int &fd ) { _fd = fd; };
		void 		setHostname(std::string const &hostname ) { _hostname = hostname; };
		void 		setUsername(std::string const &username ) { _username = username; };
		void 		setNickname(std::string const &nickname ) { _nickname = nickname; };
		void 		setRealname(std::string const &realname ) { _realname = realname; };
		void 		setPartialInput(std::string const &partialInput ) { _partialInput = partialInput; };
		void 		setRegistered(bool isRegistered) { _isRegistered = isRegistered; };
		void 		setAuth(bool isAuth) { _isAuth = isAuth; };
		void 		setOp(bool isOp) { _isOp = isOp; };
		void 		setInvisible(bool isInvisible) { _isInvisible = isInvisible; };
		void 		setWallops(bool isWallops) { _isWallops = isWallops; };

		Channel*	getChannel(const std::string channelName);
		void		joinChannel(Channel *channel);
		void		leaveChannel(Channel *channel);
		void		leaveAllChannels();
		void		sendMessage(const std::string &message);
};

#endif