#include "../includes/Client.hpp"

Client::Client(
    std::string nickname,
    std::string username,
    std::string hotsname,
    std::string mode 
){
    this->nickname = nickname;
    this->username = username;
    this->hotsname = hotsname;
    this->mode = mode;
}

Client::~Client(){
    //this->channels->clear();
}

std::string Client::get_nickname() const{
    return this->nickname;
}

std::string Client::get_hostname() const{
    return this->hotsname;
}
std::string Client::get_mode() const{
    return this->mode;
}

//std::set<std::string> get_channels;//to write


void Client::set_nickname(std::string nickname){
    this->nickname = nickname;
}
void Client::set_mode(std::string mode){
    this->mode = mode;
}

//void add_channel(std::string channel);//to write