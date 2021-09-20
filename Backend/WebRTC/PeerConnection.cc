#include <iostream>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "PeerConnection.h"
#include "../Certificate.h"
#ifdef _DEBUG
#define DBG_NEW new
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

std::string generateRandomString(int length) {

    std::string randomString;

    std::string chars(
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890"
/*        "!@#$%^&*()"
        "`~-_=+[{]}\\|;:'\",<.>/? "*/);
    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
    for(int i = 0; i < length; ++i) {
        randomString +=  chars[index_dist(rng)];
    }
    return randomString;
}


PeerConnection::PeerConnection() {

    ice_ufrag = generateRandomString(4);
    ice_pwd = generateRandomString(22);


}


// void PeerConnection::SetLocalDescription() {
//
//
//
// }

std::vector<ICECandidate> PeerConnection::SetRemoteDescription(std::string sdp, std::string type) {

    remotesdp = std::shared_ptr<SDP>(DBG_NEW SDP{});
    remotesdp->Parse(sdp);

    if(type == "offer") {
        answerMode = true;
    }

 //   ice = std::shared_ptr<ICE>(new ICE());
 //   rtpHandler = std::shared_ptr<RTPHandler>(new RTPHandler());
 //   ice->GenerateCandidates(remotesdp->)

    ICE ice;

    if(answerMode) {
        //std::cout << "Got offer running in Answer mode. Bundle Size " << remotesdp->bundle.size() << std::endl;
        if(remotesdp->mediaDescriptionList.size() >= 1) {

            int i =0;
            for( auto mdes  : remotesdp->mediaDescriptionList) {

                //std::cout << "Loop start" << std::endl;
                
                    RTPHandler* _rtpHandler = DBG_NEW RTPHandler(mdes.ssrc, mdes.mid, mdes.msid, mdes.cname, ice_ufrag, ice_pwd);
                    
                    ICECandidate candidate = ice.GenerateCandidates(mdes.rtcp_mux);
                    std::thread(&RTPHandler::Listen, _rtpHandler, candidate.connectionAddress, candidate.port).detach();
                


                    candidate.mid = mdes.mid;
                    candidate.mlineindex = i;
                    if (mdes.audio) {
                        _rtpHandler->isAudioStream = mdes.audio;
                        _rtpHandler->audioSampleRate = mdes.sampleRate;
                        _rtpHandler->audioCodec = mdes.audioCodec;
                        _rtpHandler->channels = mdes.channels;
                    }
                   candidates.push_back(candidate);
                   rtpHandler.push_back(std::shared_ptr<RTPHandler>(_rtpHandler)); 

                    //std::cout << "Loop end" << std::endl;
                
 
                i++;
            }
        } else  {

                //std::cout << "Bundle and Media size are equal." << std::endl;
                int i = 0;
                std::vector<std::string> midsDone;

                //std::cout << "bundle size is " << remotesdp->bundle.size() << std::endl;
                if(remotesdp->bundle.size() > 0) {
                    RTPHandler* _rtpHandler;
                    bool generatedFirstCandidate = false;

                    int i = 0;
                    for(auto mdes : remotesdp->mediaDescriptionList) {


                        if(!generatedFirstCandidate) {

                            _rtpHandler = DBG_NEW RTPHandler(mdes.ssrc, mdes.mid, mdes.msid, mdes.cname, ice_ufrag, ice_pwd);
                            rtpHandler.push_back(std::shared_ptr<RTPHandler>(_rtpHandler));
                            ICECandidate candidate = ice.GenerateCandidates(mdes.rtcp_mux);
                            candidate.mid = mdes.mid;
                            candidate.mlineindex = i;
                            candidates.push_back(candidate);
                            std::thread(&RTPHandler::Listen, _rtpHandler, candidate.connectionAddress, candidate.port).detach();
                            generatedFirstCandidate = true;
                        } else {
                            if(candidates.size() > 0) {
                                ICECandidate candidate = candidates[0];
                                candidate.mid = mdes.mid;
                                candidate.mlineindex = i;
                                candidates.push_back(candidate);
                            }
                        }

                        i++;
                    }


                } else {


                    for(auto mdes : remotesdp->mediaDescriptionList) {

                        RTPHandler* _rtpHandler = DBG_NEW RTPHandler(mdes.ssrc, mdes.mid, mdes.msid, mdes.cname, ice_ufrag, ice_pwd);
                        ICECandidate candidate = ice.GenerateCandidates(mdes.rtcp_mux);
                        candidate.mid = remotesdp->mediaDescriptionList[i].mid;
                        candidate.mlineindex = 0;
                        std::thread(&RTPHandler::Listen, _rtpHandler, candidate.connectionAddress, candidate.port).detach();

                    //  rtpThreads.push_back(std::thread(&RTPHandler::Listen, _rtpHandler, candidate.connectionAddress, candidate.port));
                        candidates.push_back(candidate);
                       // std::unique_lock<std::mutex> l(r->readersMutex);

                        rtpHandler.push_back(std::shared_ptr<RTPHandler>(_rtpHandler));
                        i++;
                    }

                }


        }

    } else {

        //std::cout << "[yy] Bundle and Media size are equal." << std::endl;
        //std::cout << "[yy] bundle is " << localsdp->bundle.size() << std::endl;
        if( remotesdp->mediaDescriptionList.size() >= 1) {

            int i =0;
            for( auto mdes  : remotesdp->mediaDescriptionList) {


                RTPHandler* _rtpHandler = DBG_NEW RTPHandler(mdes.ssrc, mdes.mid, mdes.msid, mdes.cname, ice_ufrag, ice_pwd);

                ICECandidate candidate = ice.GenerateCandidates(mdes.rtcp_mux);

                _rtpHandler->SetSenderMode(true);

                for(auto r : sourceConnection->rtpHandler) {
                    if(mdes.mid == r->mid) {
                            _rtpHandler->SetSendSource(r);
                            std::unique_lock<std::mutex> l(r->readersMutex);
                            r->readers.push_back(_rtpHandler);
                    }
                }
                std::thread(&RTPHandler::Listen, _rtpHandler, candidate.connectionAddress, candidate.port).detach();
//
//                 rtpThreads.push_back(std::move(rtpthread));


                candidate.mid = mdes.mid;
                candidate.mlineindex = i;
                candidates.push_back(candidate);
                rtpHandler.push_back(std::shared_ptr<RTPHandler>(_rtpHandler));


                i++;
            }
        } else {


                //std::cout << "[xx] Bundle and Media size are equal." << std::endl;
                //std::cout << "[xx] bundle is " << localsdp->bundle.size() << std::endl;

                int i = 0;
                std::vector<std::string> midsDone;

                if(localsdp->bundle.size() > 0) {
                    RTPHandler* _rtpHandler;
                    bool generatedFirstCandidate = false;

                    int i = 0;
                    for(auto mdes : remotesdp->mediaDescriptionList) {


                        if(!generatedFirstCandidate) {

                            _rtpHandler = DBG_NEW RTPHandler(mdes.ssrc, mdes.mid, mdes.msid, mdes.cname, ice_ufrag, ice_pwd);
                            rtpHandler.push_back(std::shared_ptr<RTPHandler>(_rtpHandler));
                            ICECandidate candidate = ice.GenerateCandidates(mdes.rtcp_mux);
                            candidate.mid = mdes.mid;
                            candidate.mlineindex = i;
                            candidates.push_back(candidate);

                            _rtpHandler->SetSenderMode(true);
                            for(auto r : sourceConnection->rtpHandler) {
                                if(mdes.mid == r->mid) {
                                    _rtpHandler->SetSendSource(r);
                                    std::unique_lock<std::mutex> l(r->readersMutex);
                                    r->readers.push_back(_rtpHandler);
                                    //std::cout << "[xx] [!IMPORTANT] Setting send source" << std::endl;

                                }
                            }

                            std::thread(&RTPHandler::Listen, _rtpHandler, candidate.connectionAddress, candidate.port).detach();
                            generatedFirstCandidate = true;

                        } else {
                            if(candidates.size() > 0) {
                                ICECandidate candidate = candidates[0];
                                candidate.mid = mdes.mid;
                                candidate.mlineindex = i;
                                candidates.push_back(candidate);
                            }
                        }

                        i++;
                    }


                } else {

                    for(auto mdes : remotesdp->mediaDescriptionList) {

    //                     if(std::find(midsDone.begin(), midsDone.end(), remotesdp->mediaDescriptionList[i].mid) == midsDone.end()) {
    //
    //                         if((std::find(remotesdp->bundle.begin(), remotesdp->bundle.end(), mdes.mid) != remotesdp->bundle.end()) ) {
    //
    //                             for(auto b : remotesdp->bundle) {
    //                                 midsDone.push_back(b);
    //                             }
    //
    //                         }
    //
    //                     } else {
    //                         i++;
    //                         continue;
    //                     }


                        RTPHandler* _rtpHandler = DBG_NEW RTPHandler(mdes.ssrc, mdes.mid, mdes.msid, mdes.cname, ice_ufrag, ice_pwd);
                        ICECandidate candidate = ice.GenerateCandidates(mdes.rtcp_mux);
                        candidate.mid = remotesdp->mediaDescriptionList[i].mid;
                        candidate.mlineindex = 0;

                        _rtpHandler->SetSenderMode(true);
                        for(auto r : sourceConnection->rtpHandler) {
                            if(mdes.mid == r->mid) {
                                _rtpHandler->SetSendSource(r);
                                std::unique_lock<std::mutex> l(r->readersMutex);
                                r->readers.push_back(_rtpHandler);
                                //std::cout << "[!IMPORTANT] Setting send source" << std::endl;

                            }
                        }
                        std::thread(&RTPHandler::Listen, _rtpHandler, candidate.connectionAddress, candidate.port).detach();

                        //rtpThreads.push_back(std::thread(&RTPHandler::Listen, _rtpHandler, candidate.connectionAddress, candidate.port));
                        candidates.push_back(candidate);

                        rtpHandler.push_back(std::shared_ptr<RTPHandler>(_rtpHandler));
                        i++;
                    }

                }


        }
    }

    return candidates;
}


void AddKey(std::string& dest ,std::string key, std::string value) {

    dest +=  key + "=" + value + "\n";

}

//Currently Only Audio Is Supported
//fingerprint:sha-256 EC:C4:12:A8:2F:07:93:D1:AA:34:E2:5E:1E:60:14:DB:8B:FB:9B:B3:65:FC:D6:76:8D:CC:85:DA:9A:D3:7E:5E
std::string PeerConnection::CreateAnswer() {

    //std::cout << " {ANSWER GEN}" << std::endl;
    std::string answer;

    //answerMode = true;

    AddKey(answer, "v", std::to_string(remotesdp->version));
    AddKey(answer, "o", remotesdp->origin);
    AddKey(answer, "s", remotesdp->session);
    AddKey(answer, "t", remotesdp->timing);
/*
    if(remotesdp->bundle.size() > 0) {
        std::string bundle = "BUNDLE:group";

        for(auto  b : remotesdp->bundle) {

            bundle += " " + b;

        }
        AddKey(answer, "a", bundle);
    }*/
    AddKey(answer, "a", "extmap-allow-mixed");
    AddKey(answer, "a", "msid-semantic: WMS");

    for(auto mediaDesc : remotesdp->mediaDescriptionList) {

        std::string mediaLine;
        mediaLine += mediaDesc.mediaType;
        mediaLine += " " +  mediaDesc.port;
        mediaLine += " " + mediaDesc.transportType;

        for(auto ac : mediaDesc.acceptedCodecs) {
            mediaLine += " " + ac;
        }

        AddKey(answer, "m", mediaLine);
        AddKey(answer, "c", mediaDesc.c);
        AddKey(answer, "a", "rtcp:" + mediaDesc.port + " " + mediaDesc.c);
        AddKey(answer, "a", "ice-lite");
        AddKey(answer, "a", "ice-ufrag:" + ice_ufrag);
        AddKey(answer, "a", "ice-pwd:" + ice_pwd);
        //AddKey(answer, "a", "ice-options:trickle");
        AddKey(answer, "a", "fingerprint:sha-256 " + Certificate::digest);
        AddKey(answer, "a", "setup:passive");
        AddKey(answer, "a", "mid:" + mediaDesc.mid);

        for(auto ext : mediaDesc.extensions) {
            AddKey(answer, "a", ext);
        }

        AddKey(answer, "a", "recvonly");
        AddKey(answer, "a", "rtcp-mux");

        for(auto c : mediaDesc.acceptedCodecs) {
            if(mediaDesc.codecInfo.count(c) > 0) {
                std::vector<std::string> rtpmap = (mediaDesc.codecInfo.find(c)->second);
                for(auto r : rtpmap) {
                    AddKey(answer, "a", r);
                }
            }

            if(mediaDesc.feedbackInfo.count(c) > 0) {
                std::vector<std::string> rtcpfb = (mediaDesc.feedbackInfo.find(c)->second);
                for(auto r : rtcpfb) {
                    AddKey(answer, "a", r);
                }
            }
        }

    }
    //std::cout << "{ANSER END}" << std::endl;
    return answer;


}




std::string PeerConnection::CreateOffer(std::shared_ptr<PeerConnection> pc) {

    sourceConnection = pc;

    std::string offer;

    offerMode = true;

    SDP* _remotesdp = pc->remotesdp.get();

//    memcpy(&localsdp, _remotesdp, sizeof(SDP));
    localsdp = pc->remotesdp;

    if(_remotesdp == nullptr) {

        //std::cout << "remote sdp is nullptr" << std::endl;
        return "";
    }

    AddKey(offer, "v", std::to_string(_remotesdp->version));
    AddKey(offer, "o", _remotesdp->origin);
    AddKey(offer, "s", _remotesdp->session);
    AddKey(offer, "t", _remotesdp->timing);
/*
    if(_remotesdp->bundle.size() > 0) {
        std::string bundle = "BUNDLE:group";

        for(auto  b : _remotesdp->bundle) {

            bundle += " " + b;

        }
        AddKey(offer, "a", bundle);
    }*/
    AddKey(offer, "a", "extmap-allow-mixed");
    AddKey(offer, "a", "msid-semantic: WMS");
    AddKey(offer, "a", "ice-lite");
    for(auto mediaDesc : _remotesdp->mediaDescriptionList) {

        std::string mediaLine;
        mediaLine += mediaDesc.mediaType;
        mediaLine += " " +  mediaDesc.port;
        mediaLine += " " + mediaDesc.transportType;

        for(auto ac : mediaDesc.acceptedCodecs) {
            mediaLine += " " + ac;
        }

        AddKey(offer, "m", mediaLine);
        AddKey(offer, "c", mediaDesc.c);
        AddKey(offer, "a", "rtcp:" + mediaDesc.port + " " + mediaDesc.c);
        AddKey(offer, "a", "ice-lite");
        AddKey(offer, "a", "ice-ufrag:" + ice_ufrag);
        AddKey(offer, "a", "ice-pwd:" + ice_pwd);
       // AddKey(offer, "a", "ice-options:trickle");
        AddKey(offer, "a", "fingerprint:sha-256 " + Certificate::digest);
        AddKey(offer, "a", "setup:actpass");
        AddKey(offer, "a", "mid:" + mediaDesc.mid);

        for(auto ext : mediaDesc.extensions) {
            AddKey(offer, "a", ext);
        }

        AddKey(offer, "a", "sendonly");
        AddKey(offer, "a", "rtcp-mux");

        for(auto c : mediaDesc.acceptedCodecs) {
            if(mediaDesc.codecInfo.count(c) > 0) {
                std::vector<std::string> rtpmap = (mediaDesc.codecInfo.find(c)->second);
                for(auto r : rtpmap) {
                    AddKey(offer, "a", r);
                }
            }

            if(mediaDesc.feedbackInfo.count(c) > 0) {
                std::vector<std::string> rtcpfb = (mediaDesc.feedbackInfo.find(c)->second);
                for(auto r : rtcpfb) {
                    AddKey(offer, "a", r);
                }
            }
        }

        if(mediaDesc.ssrc.size() > 1) {
            AddKey(offer, "a", "ssrc-group:FID " + std::to_string(mediaDesc.ssrc[0]) + " " + std::to_string(mediaDesc.ssrc[1]));
        }

        for(auto ssrc : mediaDesc.ssrc) {

            if(!mediaDesc.msid.empty()) {
                AddKey(offer, "a", "ssrc:" + std::to_string(ssrc) + " msid:" + mediaDesc.msid);
            }
            if(!mediaDesc.cname.empty()) {
                AddKey(offer, "a", "ssrc:" + std::to_string(ssrc) + " cname:" + mediaDesc.msid);
            }

        }

    }
    return offer;

}



PeerConnection::~PeerConnection()
{
    //for(auto r : rtpHandler) {
    //    r->Shutdown();
    //}
}



