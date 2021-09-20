#ifndef PEERCONNECTION_H_INCLUDED
#define PEERCONNECTION_H_INCLUDED

#include <iostream>
#include <thread>
#include "Sdp.h"
#include "RtpHandler.h"
#include "Ice.h"
class PeerConnection {
public:

    PeerConnection();
   ~PeerConnection();
    SDP* GetLocalDescription();
    SDP* GetRemoteDescription();

    std::vector<ICECandidate> SetRemoteDescription(std::string sdp, std::string type);
    void SetLocalDescription(std::string sdp, std::string type);

    std::string CreateOffer(std::shared_ptr<PeerConnection>);
    std::string CreateAnswer();

    std::string sourceUser;

public:
    bool answerMode = false;
    bool offerMode = false;
    std::shared_ptr<PeerConnection> sourceConnection;
    std::shared_ptr<SDP> localsdp;
    std::shared_ptr<SDP> remotesdp;
    std::vector<std::shared_ptr<RTPHandler>> rtpHandler;
    std::vector<ICECandidate> candidates;
    std::vector<std::thread> rtpThreads;
    std::string ice_ufrag;
    std::string ice_pwd;
};
#endif
