#ifndef CALLSERVER_H_INCLUDED
#define CALLSERVER_H_INCLUDED

#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <nlohmann/json.hpp>
#include "../WebRTC/PeerConnection.h"

#ifdef _DEBUG
#define DBG_NEW new
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


struct User;

using json = nlohmann::json;






class CallServer {

public:

    CallServer();
    ~CallServer();


    class Session {
    public:
        std::shared_ptr<PeerConnection> pc;
        std::shared_ptr<User>  user;
        ~Session();
        Session();
    };


    class Room {
    public:
        static std::vector<std::shared_ptr<Room>> rooms;
        static std::mutex removeRoomMutex;

        std::string name;
        std::mutex removeUserMutex;
        std::vector<std::string> users;

        CallServer* callServer;

        std::map<std::string, std::shared_ptr<Session>> sessions; // All sessions where User is sending data.

        std::map<std::string, std::vector<std::shared_ptr<Session>>> receiveSessions; //All sessions where user is requesting data.

        std::thread vadThread;
        bool shutdownVad = false;
        bool isVadShutdown = true;

        //~Room() {
        //        
        //    shutdownVad = true;
        //    while (!isVadShutdown) {

        //        std::cout << "WAITING FOR VAD TO SHUT DOWN........" << std::endl;

        //    }


        //}

        //void StartVadReporter(std::string room);

        void AddUser(std::string user) {
            std::unique_lock<std::mutex> l(removeUserMutex);
            users.push_back(user);

        };

        void RemoveUser(std::string user);
        static std::shared_ptr<Room> FindRoom(std::string room) {
            
            
            std::unique_lock<std::mutex> l(removeRoomMutex);
            for (auto r : CallServer::Room::rooms) {

                if (r->name == room) {
                
                    return r;
                
                }

            }

            return nullptr;

        }

        void CreateRoom(std::string room) {

            Room* r = DBG_NEW Room();

            Room::rooms.push_back(std::shared_ptr<Room>(r));

        }
        

        static void RemoveRoom(std::string room) {
            
            
            std::unique_lock<std::mutex> l(removeRoomMutex);



            int roomSize = rooms.size();
            for (int i = 0; i < roomSize; i++) {
            
                if (rooms[i]->name == room) {
                
                    rooms.erase(rooms.begin() + i);
                    roomSize -= 1;
                
                }
                else {
                    i++;
                }
            }

        }
    };
 



    
    std::map<std::string, std::shared_ptr<Session>> sessions; // All sessions where User is sending data.



    std::mutex disconnectMutex;

    //called when user connects
    void onConnect(std::shared_ptr<User>  user, json data);

    void onJoinRoom(std::shared_ptr<User>  user, json data);

    void broadcastUserJoin(std::shared_ptr<User>  user, std::string room);

    void onOffer(std::shared_ptr<User>  user, json data);

    void onAnswer(std::shared_ptr<User>  user, json data);

    void onICECandidate(std::shared_ptr<User>  user, json data);

    void onRequestUserList(std::shared_ptr<User>  user, json data);

    void onSetUsername(std::shared_ptr<User>  user, json data);

    void onRequestUserMedia(std::shared_ptr<User>  user, json data);

    void onRecvMediaICECandidate(std::shared_ptr<User>  user, json data);

    void onDisconnect(std::shared_ptr<User>  user, json data);

    void doAuth(std::shared_ptr<User>  user, json data);

    void StartVadReporter(std::string room);

    void onToggleVideo(std::shared_ptr<User>, json data);
    
    void onToggleAudio(std::shared_ptr<User>, json data);

};

#endif // CALLSERVER_H_INCLUDED
