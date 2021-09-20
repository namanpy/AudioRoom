#ifndef ICE_H_INCLUDED
#define ICE_H_INCLUDED

#include <iostream>
#include <vector>

class ICECandidate {
public:
    ICECandidate(double foundation,
                 int componentId,
                 std::string transport,
                 double priority,
                 std::string connectionAddress,
                 int port,
                 std::string candidateType);

    std::string ToString();
    double foundation;
    int componentId;
    std::string transport;
    double priority;
    std::string connectionAddress;
    int port;
    std::string mid;
    int mlineindex;
    std::string candidateType;

};

class ICE {

public:
    ICECandidate GenerateCandidates(bool rtcpMux);
    void FindPublicIP(std::string&);
    void FindLocalIP(std::string&);

};


#endif // ICE_H_INCLUDED
