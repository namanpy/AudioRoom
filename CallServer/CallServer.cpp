#include "CallServer.h"
#include "../WebRTC/Ice.h"
#include "../EventEmitter.cpp"
#include "../SignalServer.cpp"
#include <algorithm>

#include <stdlib.h>


#ifdef _DEBUG
#define DBG_NEW new
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

//#include <unistd.h>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>


std::vector<std::shared_ptr<CallServer::Room>> CallServer::Room::rooms;
std::mutex CallServer::Room::removeRoomMutex;
CallServer::CallServer() {


}

void CallServer::StartVadReporter(std::string room) {
    std::shared_ptr<Room> rm = Room::FindRoom(room);

    if (rm == nullptr) {
        return;
    }
    while (true) {
        //std::cout << "vad running" << std::endl;
        std::vector<std::string> usersSpeaking;

        if (rm == nullptr) {

            break;

        }

        std::unique_lock<std::mutex> dl(disconnectMutex);
        std::unique_lock<std::mutex> discLock(rm->removeUserMutex);


        for (auto usr : rm->users) {
            if (sessions.count(usr) >= 1) {


                for (auto rtpHandler : sessions.find(usr)->second->pc->rtpHandler) {

                    if (rtpHandler->vad.get() != nullptr && rtpHandler->vad->CurrentlySpeaking) {

                        usersSpeaking.push_back(usr);
                        //std::cout << (usr) << std::endl;

                    }

                }


            }


        }


        if (usersSpeaking.size() >= 0) {

            for (auto usr : rm->users) {

                json j;

                j["speakers"] = usersSpeaking;
                try {
                    sessions.find(usr)->second->user->emit("speaking", j);
                }
                catch (std::exception e) {
                    std::cout << "CAUGHT IN 4K" << std::endl;
                }
            }
        }
        dl.unlock();
        //discLock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}


void CallServer::doAuth(std::shared_ptr<User>  user, json data) {

    //set Username
    user->id = data["username"];


    Session* session = DBG_NEW Session();
    PeerConnection* pc = DBG_NEW PeerConnection();

    session->user = user;
    session->pc = std::unique_ptr<PeerConnection>(pc);
    session->pc->sourceUser = user->id;

    this->sessions.insert( { user->id, std::shared_ptr<Session>(session) } );
   // std::cout << " User with UUID " << user->id << " joined. \nAdded user to Session" << std::endl;


}

void CallServer::onToggleVideo(std::shared_ptr<User> user, json data)
{

    
    std::shared_ptr<Room> room = Room::FindRoom(data["room"]);

    std::unique_lock<std::mutex> discLock(room->removeUserMutex);


    if (room != nullptr) {
            
        bool video = data["video"];
        
        for (auto user_ : room->users) {
        
            json j;
            j["video"] = video;
            j["user"] = user->id;
            sessions.at(user_)->user->emit("~mutevideo", j);

       
        }


    }



}

void CallServer::onToggleAudio(std::shared_ptr<User> user, json data)
{


    std::shared_ptr<Room> room = Room::FindRoom(data["room"]);

    std::unique_lock<std::mutex> discLock(room->removeUserMutex);

    if (room != nullptr) {

        bool audio = data["audio"];

        for (auto user_ : room->users) {

            json j;
            j["audio"] =audio;
            j["user"] = user->id;

            sessions.at(user_)->user->emit("~muteaudio", j);


        }


    }





}

void CallServer::onJoinRoom(std::shared_ptr<User>  user, json data) {

    std::shared_ptr<Room> room = Room::FindRoom(data["room"]);

    if(room != nullptr) {

        //Add user to existing room of that name
        for (auto u : room->users) {
            if (u == user->id) {
                return;
            }

        }
        room->AddUser(user->id);
        //Start peerConnection to receive user data and set room name in session.
        //sessions.find(user->id)->second->room = data["room"];

        //std::cout << std::endl << std::endl << "======= User Broadcasting =====" << std::endl <<std::endl;
        //this->broadcastUserJoin(user, data["room"]);

    } else {
        // Create New ROOM;


        std::shared_ptr<Room> room = std::shared_ptr<Room>(DBG_NEW Room());
        room->name = data["room"];
        std::unique_lock<std::mutex> l(Room::removeRoomMutex);
        Room::rooms.push_back(room);

        //sessions.find(user->id)->second->room = data["room"];
        room->AddUser(user->id);

        //room->callServer = (this);

        //std::thread(&CallServer::StartVadReporter, this, data["room"]).detach();
        //room->vadThread.detach();


    }
    user->room = data["room"];

}


void CallServer::broadcastUserJoin(std::shared_ptr<User>  user, std::string room) {

    std::shared_ptr<Room> rm = Room::FindRoom(room);
    

    if(rm != nullptr) {

        std::vector<std::string> users = rm->users;

        for(int i = 0; i < users.size(); i++) {

            if(sessions.count(users.at(i)) > 0) {

                if(users.at(i) != user->id) {

                    json data;
                    data["user"] = user->id;
                    sessions.find(users.at(i))->second->user->emit("newUser", data);
                }


            }
        }
    }
}




void CallServer::onOffer(std::shared_ptr<User>  user, json data) {
    //std::cout << " Offer from User with UUID " << user->id << " \n ======= Remote Description =======\n" << data["offer"] << std::endl;

    if(data["offer"]["type"] == "offer") {

        std::vector<ICECandidate> candidates = sessions.find(user->id)->second->pc->SetRemoteDescription(data["offer"]["sdp"], "offer");



        std::string answer =  sessions.find(user->id)->second->pc->CreateAnswer();

        std::cout << "answer " << answer << std::endl;

        json j;
        j["answer"] = answer;
        user->emit("answer", j);


        std::cout << "CANDIDATE SIZE IS > "<<  candidates.size();


        for(auto can : candidates) {
            std::cout << "SetRemoteDescription mid " << can.mid << " mlineindex" << can.mlineindex<< std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            json j;
            j["iceCandidate"] = can.ToString();
            j["index"] = can.mlineindex;
            j["mid"] = can.mid;
            user->emit("iceCandidate", j);
        }
        std::cout << "offer in room > " << data["room"] << std::endl;
        this->broadcastUserJoin(user, data["room"]);

//         if(!this->sendMedia) {
//            user->emit("iceCandidate", j);
//         } else {
//             std::cout << "[!~!] sendICECandidate" << std::endl;
//             j["user"] = this->sourceUser;
//             user->emit("~iceCandidate", j);
//         }313

    }


}


void CallServer::onAnswer(std::shared_ptr<User>  user, json data) {
    std::cout << "Answer from User with UUID " << user->id << " \n ======= Remote Description =======\n" << "" << std::endl;

    if(data["user"] != nullptr) {

  //          std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
 //           std::cout << "WEEEEEEEEEEEe\n";
//            std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n";


        std::shared_ptr<Room> rm = Room::FindRoom(data["room"]);

        if (rm == nullptr) {
            return;
        }

        for(int i=0; i < rm->receiveSessions.find(user->id)->second.size(); i++) {



            if(rm->receiveSessions.find(user->id)->second.at(i)->pc->sourceUser == data["user"]) {
      

                std::vector<ICECandidate> candidates =  rm->receiveSessions.find(user->id)->second.at(i)->pc->SetRemoteDescription(data["answer"]["sdp"], "answer");


                for(auto can : candidates) {
                    //std::cout << "SetRemoteDescription mid " << can.mid << " mlineindex" << can.mlineindex<< std::endl;

                    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
                    json j;
                    j["iceCandidate"] = can.ToString();
                    j["index"] = can.mlineindex;
                    j["mid"] = can.mid;
                    j["user"] = data["user"];
                    user->emit(data["user"].get<std::string>() + "~iceCandidate", j);
                }
            }

        }

    } else {
        std::cout << "Data USER IS NULL PTR!!!!" <<std::endl;
    }


}


void CallServer::onICECandidate(std::shared_ptr<User>  user, json data) {

    //sessions.find(user->id)->second->pc->setICECandidate(data["sdpMid"], (int) data["sdpMLineIndex"], data["candidate"]);
}

void CallServer::onRequestUserList(std::shared_ptr<User>  user, json data) {

    std::shared_ptr<Room> rm = Room::FindRoom(data["room"]);

    std::cout << "onRequestUserList() requested userlist " << std::endl;

    if (rm == nullptr) {
        return;
    }



    json j;
    j["users"] = rm->users;

    for (auto j : rm->users) {
        std::cout << "user in room : " << j << std::endl;
    }
    
    user->emit("userList", j);

}


void CallServer::onSetUsername(std::shared_ptr<User>  user, json data) {

    user->id = data["username"];
}


void CallServer::onRequestUserMedia(std::shared_ptr<User>  user, json data) {


    std::cout << ("[~] Offer") << std::endl;
    std::cout << data.dump() << std::endl;
    if(sessions.count(data["user"]) > 0) {

        std::cout << data["user"] << " is requested user , then on requesting is " << user->id << std::endl;


        std::shared_ptr<Session> session = std::shared_ptr<Session>(DBG_NEW Session());

        std::shared_ptr<PeerConnection> pc(DBG_NEW PeerConnection());

        session->pc = pc;
        session->pc->sourceUser = data["user"];
        session->user = sessions.find(data["user"])->second->user;


        std::string offr = session->pc.get()->CreateOffer(sessions.find(data["user"])->second->pc);

        json j;
        j["offer"] = offr;
        j["user"] = data["user"];
        std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$ " << data["user"].get<std::string>() << std::endl;
        user->emit(data["user"].get<std::string>() + "~offer", j);

        std::shared_ptr<Room> rm = Room::FindRoom(data["room"]);

        if (rm == nullptr) {
            return;
        }


        if(rm->receiveSessions.count(user->id) > 0) {
         
            rm->receiveSessions.find(user->id)->second.push_back(std::shared_ptr<Session>(session));

        } else {
            std::vector<std::shared_ptr<Session>> s;

            s.push_back(std::shared_ptr<Session>(session));

            rm->receiveSessions.insert({ user->id , s});
        }


    }



}

void CallServer::onRecvMediaICECandidate(std::shared_ptr<User>  user, json data) {



}

void CallServer::onDisconnect(std::shared_ptr<User>  user, json data) {
    std::unique_lock<std::mutex> discLock(disconnectMutex);







    std::shared_ptr<Room> rm = Room::FindRoom(user->room);
    if (rm == nullptr) {
        return;
    }
    std::unique_lock<std::mutex> l(rm->removeUserMutex);


    rm->RemoveUser(user->id);

    if (rm->users.size() < 1) {
    
        std::cout << "Room is empty removing room;" << std::endl;

        Room::RemoveRoom(rm->name);
    
    }
    else {
        for (auto s : sessions) {
            json j;
            j["user"] = user->id;
            std::cout << "Sending disconnect event " << user->id + "~disconnect to " << s.second->user->id << std::endl;
            try {
                s.second->user->emit(user->id + "~disconnect", j);
            }
            catch (std::exception err) {
            
            }
        }
    }


    if (sessions.count(user->id) > 0) {


        sessions.erase(user->id);

    }

    //_CrtDumpMemoryLeaks();

}
CallServer::~CallServer() {

}

CallServer::Session::Session() {

}

CallServer::Session::~Session()
{
    std::cout << "Delete Session Called" << std::endl;

}


void CallServer::Room::RemoveUser(std::string user) {

    std::cout << "RemoveUser() : Disconnecting User  : " << user << std::endl;

    int i = 0;
    for (auto u : users) {
        if (user == u) {
            std::cout << "RemoveUser() : Deleting user from room list  :  " << u << std::endl;

            users.erase(users.begin() + i);
        }
        else {
            std::cout << "RemoveUser() : Not deleting this user from room list  :  " << u << std::endl;

        }
       
        i++;
    }


    while(receiveSessions.count(user) > 0) {

        std::cout << "RECEIVE SESSIONS REEE = " << receiveSessions.size() << std::endl;
        receiveSessions.erase(user);
        std::cout << "RECEIVE SESSIONS after the REEE = " << receiveSessions.size() << std::endl;



    }
    int j = 0;
    for (auto s : receiveSessions) {

        int i = 0;
        std::vector<std::shared_ptr<Session>>  sources;
        int vecSize = s.second.size();
        for (int i = 0; i  <  vecSize; i++) {
            //std::cout << "ran " +  std::to_string(i + 1) + " Times <======== numun\n" ;
            //std::cout << "RemoveUser() :  >>> " << s.first;
            //std::cout << " >>>  getting data from "  <<  s.second[i]->user->id << std::endl;
            if (s.second[i]->user->id == user) {
                std::cout <<  "############# DELETING " << s.first << " >>>  getting data from " << s.second[i]->user->id << std::endl;

                //receiveSessions.at(s.first).erase(receiveSessions.at(s.first).begin() + i);
                i++;
            }
            else {
                sources.push_back(receiveSessions.at(s.first)[i]);
                i++;
            }
            
        }
        receiveSessions.at(s.first) = sources;
        //receiveSessions.at(s.first).erase(remove(receiveSessions.at(s.first).begin(), receiveSessions.at(s.first).end(), itr), receiveSessions.at(s.first).end());
        j++;

    }

    for (auto s : receiveSessions) {
    
        std::string connectedSources = "";
        for (auto sources : s.second) {
        
        
            if (user == sources->user->id) {
            
                connectedSources += "[SHOULD'VE BEEN DELETED] " + sources->user->id + "\n";

            }
            else {
                connectedSources += "[!!!] " + sources->user->id + "\n";

            
            }
            
        }
        std::cout << "[!IMPORTANT] Key is " + s.first + " : \n" + connectedSources;
    }


  /*  while (sessions.count(user) > 0) {

        sessions.erase(user);

    }*/

    for (auto usr : users) {
        std::cout << user << " << :) " << std::endl;
    }


    std::cout << std::endl << std::endl;
    std::cout << "Deletion finished " << std::endl;
    for (auto s : receiveSessions) {
        std::cout << "# " << s.first << std::endl;
        for (auto k : s.second) {
        
            std::cout << "##> " << k->user->id << std::endl;
        
        }
    
    }

};


