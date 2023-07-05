#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

 class Client
 {
    public:
        Client(
            std::string nickname,
            std::string username,
            std::string hotsname,
            std::string mode
        );
        ~Client();
    public:
        //getters
        std::string get_nickname() const;
        std::string get_hostname() const;
        std::string get_mode() const;
        //std::set<std::string> get_channels;

        //setters
        void set_nickname(std::string nickname);
        void set_mode(std::string mode);
        //void add_channel(std::string channel);
    private:
        std::string _nickname;
        std::string _username;
        std::string _hotsname;
        std::string _mode;

        //std::set<std::string> channels;
 };

 #endif