#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED

#include <iostream>
#include <map>
#include <nlohmann/json.hpp>


struct User;

class Session {

    std::unique_ptr<PeerConnection> pc;
    User* user;
    std::string room;
    ~Session();
    Session();
};

#endif // SESSION_H_INCLUDED
