#ifndef RTPPACKET_H_INCLUDED
#define RTPPACKET_H_INCLUDED

#include <iostream>
#include <vector>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

typedef boost::shared_mutex Lock;
typedef boost::unique_lock< Lock >  WriteLock;
typedef boost::shared_lock< Lock >  ReadLock;


class RTPPacket {
public:

    unsigned char data[1500];
    int packetLen;
    unsigned int id;
    bool rtcp = false;
    bool unprotected = false;
    RTPPacket(); /*{
        id = 0;
        packetLen = 0;
        data = std::make_unique<unsigned char>(new unsigned char[1500]);
    
    };*/
    ~RTPPacket();
};



#endif // RTPPACKET_H_INCLUDED
