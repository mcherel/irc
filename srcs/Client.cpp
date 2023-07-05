#include "../includes/Client.hpp"

Client::Client(
    std::string nickname,
    std::string username,
    std::string hotsname,
    std::string mode 
){
    this->_nickname = nickname;
    this->_username = username;
    this->_hotsname = hotsname;
    this->_mode = mode;
}

Client::~Client(){
    //this->channels->clear();
}

std::string Client::get_nickname() const{
    return this->_nickname;
}

std::string Client::get_hostname() const{
    return this->_hotsname;
}
std::string Client::get_mode() const{
    return this->_mode;
}

//std::set<std::string> get_channels;//to write


void Client::set_nickname(std::string nickname){
    this->_nickname = nickname;
}
void Client::set_mode(std::string mode){
    this->_mode = mode;
}

//void add_channel(std::string channel);//to write