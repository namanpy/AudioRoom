#include <iostream>
#include <vector>
#include "Sdp.h"
#include "debug.h"
#include <opus.h>



// OFFER
//
// App.js:202 v=0
// o=- 1746110361190241258 2 IN IP4 127.0.0.1
// s=-
// t=0 0
// a=group:BUNDLE 0 1
// a=extmap-allow-mixed
// a=msid-semantic: WMS BGS5sZMjFgC8QjrHWrpw8tFM9vOHuC3uoz0C
// m=audio 9 UDP/TLS/RTP/SAVPF 111 103 104 9 0 8 106 105 13 110 112 113 126
// c=IN IP4 0.0.0.0
// a=rtcp:9 IN IP4 0.0.0.0
// a=ice-ufrag:BL+E
// a=ice-pwd:q9sNt7/io5Cg3tDwx0sgWifX
// a=ice-options:trickle
// a=fingerprint:sha-256 D5:E8:36:CB:31:93:E6:A8:3E:48:96:D6:D6:79:87:20:39:7D:80:04:1E:73:65:01:27:5A:34:99:4E:0D:BC:A2
// a=setup:actpass
// a=mid:0
// a=extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level
// a=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
// a=extmap:3 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01
// a=extmap:4 urn:ietf:params:rtp-hdrext:sdes:mid
// a=extmap:5 urn:ietf:params:rtp-hdrext:sdes:rtp-stream-id
// a=extmap:6 urn:ietf:params:rtp-hdrext:sdes:repaired-rtp-stream-id
// a=sendrecv
// a=msid:BGS5sZMjFgC8QjrHWrpw8tFM9vOHuC3uoz0C 0c2963ee-910e-458d-83c5-f550785dbd1a
// a=rtcp-mux
// a=rtpmap:111 opus/48000/2
// a=rtcp-fb:111 transport-cc
// a=fmtp:111 minptime=10;useinbandfec=1
// a=rtpmap:103 ISAC/16000
// a=rtpmap:104 ISAC/32000
// a=rtpmap:9 G722/8000
// a=rtpmap:0 PCMU/8000
// a=rtpmap:8 PCMA/8000
// a=rtpmap:106 CN/32000
// a=rtpmap:105 CN/16000
// a=rtpmap:13 CN/8000
// a=rtpmap:110 telephone-event/48000
// a=rtpmap:112 telephone-event/32000
// a=rtpmap:113 telephone-event/16000
// a=rtpmap:126 telephone-event/8000
// a=ssrc:2273395560 cname:Gghf2WlkpwIu8n1l
// a=ssrc:2273395560 msid:BGS5sZMjFgC8QjrHWrpw8tFM9vOHuC3uoz0C 0c2963ee-910e-458d-83c5-f550785dbd1a
// a=ssrc:2273395560 mslabel:BGS5sZMjFgC8QjrHWrpw8tFM9vOHuC3uoz0C
// a=ssrc:2273395560 label:0c2963ee-910e-458d-83c5-f550785dbd1a
// m=video 9 UDP/TLS/RTP/SAVPF 96 97 98 99 100 101 102 121 127 120 125 107 108 109 35 36 124 119 123
// c=IN IP4 0.0.0.0
// a=rtcp:9 IN IP4 0.0.0.0
// a=ice-ufrag:BL+E
// a=ice-pwd:q9sNt7/io5Cg3tDwx0sgWifX
// a=ice-options:trickle
// a=fingerprint:sha-256 D5:E8:36:CB:31:93:E6:A8:3E:48:96:D6:D6:79:87:20:39:7D:80:04:1E:73:65:01:27:5A:34:99:4E:0D:BC:A2
// a=setup:actpass
// a=mid:1
// a=extmap:14 urn:ietf:params:rtp-hdrext:toffset
// a=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
// a=extmap:13 urn:3gpp:video-orientation
// a=extmap:3 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01
// a=extmap:12 http://www.webrtc.org/experiments/rtp-hdrext/playout-delay
// a=extmap:11 http://www.webrtc.org/experiments/rtp-hdrext/video-content-type
// a=extmap:7 http://www.webrtc.org/experiments/rtp-hdrext/video-timing
// a=extmap:8 http://www.webrtc.org/experiments/rtp-hdrext/color-space
// a=extmap:4 urn:ietf:params:rtp-hdrext:sdes:mid
// a=extmap:5 urn:ietf:params:rtp-hdrext:sdes:rtp-stream-id
// a=extmap:6 urn:ietf:params:rtp-hdrext:sdes:repaired-rtp-stream-id
// a=sendrecv
// a=msid:BGS5sZMjFgC8QjrHWrpw8tFM9vOHuC3uoz0C d30ac34f-3b56-4bbe-a500-08f8b2a8d3f0
// a=rtcp-mux
// a=rtcp-rsize
// a=rtpmap:96 VP8/90000
// a=rtcp-fb:96 goog-remb
// a=rtcp-fb:96 transport-cc
// a=rtcp-fb:96 ccm fir
// a=rtcp-fb:96 nack
// a=rtcp-fb:96 nack pli
// a=rtpmap:97 rtx/90000
// a=fmtp:97 apt=96
// a=rtpmap:98 VP9/90000
// a=rtcp-fb:98 goog-remb
// a=rtcp-fb:98 transport-cc
// a=rtcp-fb:98 ccm fir
// a=rtcp-fb:98 nack
// a=rtcp-fb:98 nack pli
// a=fmtp:98 profile-id=0
// a=rtpmap:99 rtx/90000
// a=fmtp:99 apt=98
// a=rtpmap:100 VP9/90000
// a=rtcp-fb:100 goog-remb
// a=rtcp-fb:100 transport-cc
// a=rtcp-fb:100 ccm fir
// a=rtcp-fb:100 nack
// a=rtcp-fb:100 nack pli
// a=fmtp:100 profile-id=2
// a=rtpmap:101 rtx/90000
// a=fmtp:101 apt=100
// a=rtpmap:102 H264/90000
// a=rtcp-fb:102 goog-remb
// a=rtcp-fb:102 transport-cc
// a=rtcp-fb:102 ccm fir
// a=rtcp-fb:102 nack
// a=rtcp-fb:102 nack pli
// a=fmtp:102 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42001f
// a=rtpmap:121 rtx/90000
// a=fmtp:121 apt=102
// a=rtpmap:127 H264/90000
// a=rtcp-fb:127 goog-remb
// a=rtcp-fb:127 transport-cc
// a=rtcp-fb:127 ccm fir
// a=rtcp-fb:127 nack
// a=rtcp-fb:127 nack pli
// a=fmtp:127 level-asymmetry-allowed=1;packetization-mode=0;profile-level-id=42001f
// a=rtpmap:120 rtx/90000
// a=fmtp:120 apt=127
// a=rtpmap:125 H264/90000
// a=rtcp-fb:125 goog-remb
// a=rtcp-fb:125 transport-cc
// a=rtcp-fb:125 ccm fir
// a=rtcp-fb:125 nack
// a=rtcp-fb:125 nack pli
// a=fmtp:125 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e01f
// a=rtpmap:107 rtx/90000
// a=fmtp:107 apt=125
// a=rtpmap:108 H264/90000
// a=rtcp-fb:108 goog-remb
// a=rtcp-fb:108 transport-cc
// a=rtcp-fb:108 ccm fir
// a=rtcp-fb:108 nack
// a=rtcp-fb:108 nack pliboost::split(values, attribute, boost::is_any_of(":"));
// a=fmtp:108 level-asymmetry-allowed=1;packetization-mode=0;profile-level-id=42e01f
// a=rtpmap:109 rtx/90000
// a=fmtp:109 apt=108
// a=rtpmap:35 AV1X/90000
// a=rtcp-fb:35 goog-remb
// a=rtcp-fb:35 transport-cc
// a=rtcp-fb:35 ccm fir
// a=rtcp-fb:35 nack
// a=rtcp-fb:35 nack pli
// a=rtpmap:36 rtx/90000
// a=fmtp:36 apt=35
// a=rtpmap:124 red/90000
// a=rtpmap:119 rtx/90000
// a=fmtp:119 apt=124
// a=rtpmap:123 ulpfec/90000
// a=ssrc-group:FID 723721093 3946288423
// a=ssrc:723721093 cname:Gghf2WlkpwIu8n1l
// a=ssrc:723721093 msid:BGS5sZMjFgC8QjrHWrpw8tFM9vOHuC3uoz0C d30ac34f-3b56-4bbe-a500-08f8b2a8d3f0
// a=ssrc:723721093 mslabel:BGS5sZMjFgC8QjrHWrpw8tFM9vOHuC3uoz0C
// a=ssrc:723721093 label:d30ac34f-3b56-4bbe-a500-08f8b2a8d3f0
// a=ssrc:3946288423 cname:Gghf2WlkpwIu8n1l
// a=ssrc:3946288423 msid:BGS5sZMjFgC8QjrHWrpw8tFM9vOHuC3uoz0C d30ac34f-3b56-4bbe-a500-08f8b2a8d3f0
// a=ssrc:3946288423 mslabel:BGS5sZMjFgC8QjrHWrpw8tFM9vOHuC3uoz0C
// a=ssrc:3946288423 label:d30ac34f-3b56-4bbe-a500-08f8b2a8d3f0
//
// App.js:49 fn async data => {
//       try {
//         let sdp = await this.peerConnection.setRemoteDescription({
//           type: "answer",
//           sdp: data.answer
//         });
//         console.log("Remote description…
// App.js:49 fn async data => {
//       try {
//         console.log(data);
//         await this.peerConnection.addIceCandidate(new RTCIceCandidate({
//           candidate: data.iceCandidate,
//           sdpMid: data.mid,
//     …
// App.js:49 fn async data => {
//       for (let i = 0; i < data.users.length; i++) {
//         this.remotepeers.current[data.users[i]] = {};
//         await this.getVideoFromRemotePeers(data.users[i]);
//       }
//
//       con…
// App.js:49 fn async data => {
//       this.remotepeers.current[data.user] = {};
//       await this.getVideoFromRemotePeers(data.user);
//     }
// App.js:63 answer
// App.js:223 Remote description set.
//  v=0
// o=- 8974817097568844537 2 IN IP4 127.0.0.1
// s=-
// t=0 0
// a=group:BUNDLE 0 1
// a=extmap-allow-mixed
// a=msid-semantic: WMS
// m=audio 9 UDP/TLS/RTP/SAVPF 111 110
// c=IN IP4 0.0.0.0
// a=rtcp:9 IN IP4 0.0.0.0
// a=ice-ufrag:rUiA
// a=ice-pwd:u4x4WrlKZmy2cqNJWMcE/0K1
// a=ice-options:trickle
// a=fingerprint:sha-256 BC:CD:A4:11:35:40:14:C0:0E:F2:F3:7A:DD:4C:BB:67:F2:33:83:80:CE:1B:85:71:C2:77:3E:65:29:4F:3D:20
// a=setup:active
// a=mid:0
// a=extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level
// a=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
// a=extmap:3 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01
// a=recvonly
// a=rtcp-mux
// a=rtpmap:111 opus/48000/2
// a=rtcp-fb:111 transport-cc
// a=fmtp:111 minptime=10;useinbandfec=1
// a=rtpmap:110 telephone-event/48000
// m=video 9 UDP/TLS/RTP/SAVPF 96 97 98 99 100 101 102 121 127 120 125 107 108 109 35 36 124 119 123
// c=IN IP4 0.0.0.0
// a=rtcp:9 IN IP4 0.0.0.0
// a=ice-ufrag:rUiA
// a=ice-pwd:u4x4WrlKZmy2cqNJWMcE/0K1
// a=ice-options:trickle
// a=fingerprint:sha-256 BC:CD:A4:11:35:40:14:C0:0E:F2:F3:7A:DD:4C:BB:67:F2:33:83:80:CE:1B:85:71:C2:77:3E:65:29:4F:3D:20
// a=setup:active
// a=mid:1
// a=extmap:14 urn:ietf:params:rtp-hdrext:toffset
// a=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
// a=extmap:13 urn:3gpp:video-orientation
// a=extmap:3 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01
// a=extmap:12 http://www.webrtc.org/experiments/rtp-hdrext/playout-delay
// a=extmap:11 http://www.webrtc.org/experiments/rtp-hdrext/video-content-type
// a=extmap:7 http://www.webrtc.org/experiments/rtp-hdrext/video-timing
// a=extmap:8 http://www.webrtc.org/experiments/rtp-hdrext/color-space
// a=recvonly
// a=rtcp-mux
// a=rtcp-rsize
// a=rtpmap:96 VP8/90000
// a=rtcp-fb:96 goog-remb
// a=rtcp-fb:96 transport-cc
// a=rtcp-fb:96 ccm fir
// a=rtcp-fb:96 nack
// a=rtcp-fb:96 nack pli
// a=rtpmap:97 rtx/90000
// a=fmtp:97 apt=96
// a=rtpmap:98 VP9/90000
// a=rtcp-fb:98 goog-remb
// a=rtcp-fb:98 transport-cc
// a=rtcp-fb:98 ccm fir
// a=rtcp-fb:98 nack
// a=rtcp-fb:98 nack pli
// a=fmtp:98 profile-id=0
// a=rtpmap:99 rtx/90000
// a=fmtp:99 apt=98
// a=rtpmap:100 VP9/90000
// a=rtcp-fb:100 goog-remb
// a=rtcp-fb:100 transport-cc
// a=rtcp-fb:100 ccm fir
// a=rtcp-fb:100 nack
// a=rtcp-fb:100 nack pli
// a=fmtp:100 profile-id=2
// a=rtpmap:101 rtx/90000
// a=fmtp:101 apt=100
// a=rtpmap:102 H264/90000
// a=rtcp-fb:102 goog-remb
// a=rtcp-fb:102 transport-cc
// a=rtcp-fb:102 ccm fir
// a=rtcp-fb:102 nack
// a=rtcp-fb:102 nack pli
// a=fmtp:102 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42001f
// a=rtpmap:121 rtx/90000
// a=fmtp:121 apt=102
// a=rtpmap:127 H264/90000
// a=rtcp-fb:127 goog-remb
// a=rtcp-fb:127 transport-cc
// a=rtcp-fb:127 ccm fir
// a=rtcp-fb:127 nack
// a=rtcp-fb:127 nack pli
// a=fmtp:127 level-asymmetry-allowed=1;packetization-mode=0;profile-level-id=42001f
// a=rtpmap:120 rtx/90000
// a=fmtp:120 apt=127
// a=rtpmap:125 H264/90000
// a=rtcp-fb:125 goog-remb
// a=rtcp-fb:125 transport-cc
// a=rtcp-fb:125 ccm fir
// a=rtcp-fb:125 nack
// a=rtcp-fb:125 nack pli
// a=fmtp:125 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e01f
// a=rtpmap:107 rtx/90000
// a=fmtp:107 apt=125
// a=rtpmap:108 H264/90000
// a=rtcp-fb:108 goog-remb
// a=rtcp-fb:108 transport-cc
// a=rtcp-fb:108 ccm fir
// a=rtcp-fb:108 nack
// a=rtcp-fb:108 nack pli
// a=fmtp:108 level-asymmetry-allowed=1;packetization-mode=0;profile-level-id=42e01f
// a=rtpmap:109 rtx/90000
// a=fmtp:109 apt=108
// a=rtpmap:35 AV1X/90000
// a=rtcp-fb:35 goog-remb
// a=rtcp-fb:35 transport-cc
// a=rtcp-fb:35 ccm fir
// a=rtcp-fb:35 nack
// a=rtcp-fb:35 nack pli
// a=rtpmap:36 rtx/90000
// a=fmtp:36 apt=35
// a=rtpmap:124 red/90000
// a=rtpmap:119 rtx/90000
// a=fmtp:119 apt=124
// a=rtpmap:123 ulpfec/90000
//
// App.js:63 iceCandidate
//

void splitFirstOf(std::vector<std::string>& ls, std::string s, std::string c) {

    std::string::iterator it;

    std::string currentString;

    bool firstFound = false;

    for (int i=0; i  < s.size(); i++) {

        if(s[i] == c.c_str()[0] && !firstFound) {

            ls.push_back(currentString);
            currentString = "";
            firstFound = true;

        } else {
            currentString += s[i];
        }

    }

    ls.push_back(currentString);

}



MediaDescription::MediaDescription(std::string value)
{
    std::vector<std::string> values;

    boost::split(values, value, boost::is_any_of(" "));

    mediaType = values[0];
    port = values[1];
    transportType = values[2];
/*
    bool foundCompatibleCodec = false;

    for(int i=3; i< values.size(); i++) {
        if(values[i] == "110" || values[i] == "111") { //telephone-event
            foundCompatibleCodec = true;
            acceptedCodecs.push_back(values[i]);
        }
        if(values[i] == "96" || values[i] == "98" || values[i] == "127"1) { //telephone-event
            foundCompatibleCodec = true;
            acceptedCodecs.push_back(values[i]);
        }
    }
    if(!foundCompatibleCodec) {
        //throw err;
    }*/


}

int MediaDescription::AddAttribute(std::string attribute)
{
    //std::cout << "Add attr. " << attribute << std::endl;
    if(boost::starts_with(attribute, "mid")) {

        std::vector<std::string> values;
        boost::split(values, attribute, boost::is_any_of(":"));
        mid = values[1];

    } if(boost::starts_with(attribute, "msid:")) {

        std::vector<std::string> values;
        boost::split(values, attribute, boost::is_any_of(":"));
        msid = values[1];

    } else if(boost::starts_with(attribute, "ssrc:")) {

        std::vector<std::string> values;
        splitFirstOf(values, attribute, ":");

        std::vector<std::string> ssrcAndMsid;
        boost::split(ssrcAndMsid, values[1], boost::is_any_of(" "));

        unsigned int  _ssrc = (unsigned int) stoul(ssrcAndMsid[0]);

        //std::cout << "ssrcAndMsid is " << _ssrc <<std::endl;

        if(ssrc.size() > 0) {
            bool ssrcExists = false;
            //std::cout << "ssrcExists?" << std::endl;
            for(auto __ssrc : ssrc) {
                if(__ssrc == _ssrc) {
                    ssrcExists = true;
                    break;

                }
            }
             //std::cout << ssrcExists << " ssrcExists" << std::endl;
            if(!ssrcExists) {
                ssrc.push_back(_ssrc);
            }

        } else {
            //std::cout << "Add a new ssrc." << std::endl;
            ssrc.push_back(_ssrc);
        }

        if(ssrcAndMsid.size() > 1) {
            //std::cout << "1235" << std::endl;
            if(boost::starts_with(ssrcAndMsid[1], "cname:")) {
                std::vector<std::string> _values;
                splitFirstOf(_values, ssrcAndMsid[1], ":");

                cname = _values[0];

            }
            //std::cout << "1235" << std::endl;

        }

    } else if(boost::starts_with(attribute, "ice-ufrag")) {

        std::vector<std::string> values;
        boost::split(values, attribute, boost::is_any_of(":"));
        ice_ufrag = values[1];

    } else if(boost::starts_with(attribute, "ice-pwd")){



        std::vector<std::string> values;
        boost::split(values, attribute, boost::is_any_of(":"));
        ice_pwd = values[1];
        //std::cout << "ICE PWD IS " <<  ice_pwd << std::endl;
        //std::cout << std::endl << std::endl;
    } else if(boost::starts_with(attribute, "rtpmap")) {
        std::vector<std::string> values;
        boost::split(values, attribute, boost::is_any_of(":"));

        std::vector<std::string> codecIdAndCodecValue;
        if(values.size() > 1) {
            boost::split(codecIdAndCodecValue, values[1], boost::is_any_of(" "));

            if(codecIdAndCodecValue.size() > 1) {

                if(codecIdAndCodecValue[1] == "H264/90000" || codecIdAndCodecValue[1] == "VP8/90000" || codecIdAndCodecValue[1] == "VP9/90000" || codecIdAndCodecValue[1] == "opus/48000/2" ) {
                    acceptedCodecs.push_back(codecIdAndCodecValue[0]);
                }


            }


            if(codecIdAndCodecValue[1] == "opus/48000/2") {

                audio = true;
                audioCodec = Codec::Opus;
                sampleRate = 48000;
                channels = 2;
            }
            if(codecIdAndCodecValue[1] == "opus/48000") {

                audio = true;
                audioCodec = Codec::Opus;
                sampleRate = 48000;
                channels = 1;
            }

        }



        for(auto codec : acceptedCodecs) {
        if(boost::starts_with(values[1],  codec + " ")) {
                if(codecInfo.count(codec) > 0) {
                    codecInfo.find(codec)->second.push_back(attribute);
                } else {
                    std::vector<std::string> a;
                    a.push_back(attribute);
                    codecInfo.insert({ codec , a});

                }
            }
        }


    } else if(boost::starts_with(attribute, "rtcp-fb")) {

        std::vector<std::string> values;
        boost::split(values, attribute, boost::is_any_of(":"));

        std::vector<std::string> values2;
        splitFirstOf(values2, attribute, " ");


        for(auto codec : acceptedCodecs) {
            if(boost::starts_with(values[1],  codec + " ")) {
                if(feedbackInfo.count(codec) > 0) {
                    feedbackInfo.find(codec)->second.push_back(attribute);
                } else {
                    std::vector<std::string> a;
                    a.push_back(attribute);
                    feedbackInfo.insert({ codec , a });

                }
            }
        }

    } else if(boost::starts_with(attribute, "extmap")) {

        extensions.push_back(attribute);

    } else if(boost::starts_with(attribute, "fingerprint")) {

        std::vector<std::string> values;
        splitFirstOf(values, attribute, ":");

        std::vector<std::string> encAndPrint;
        boost::split(encAndPrint, values[1], boost::is_any_of(" "));

        fingerprintEncryption = encAndPrint[0];
        fingerprint = encAndPrint[1];


    } /*else if(boost::starts_with(attribute, "ssrc")) {
        std::vector<std::string> values;
        boost::split(values, attribute, boost::is_any_of(":"));

        for(auto codec : acceptedCodecs) {
            if(boost::starts_with(values[1],  codec + " ")) {
                if(feedbackInfo.count(codec) > 0) {
                    feedbackInfo.find(codec)->second.push_back(attribute);
                } else {
                    std::vector<std::string> a;
                    a.push_back(attribute);
                    feedbackInfo.insert({ codec , a });

                }
            }
        }

    }*/ else if(boost::starts_with(attribute, "rtcp-mux")) {

        rtcp_mux = true;

    }


    return 0;
}


std::string removeCharacters(std::string S, const char* c)
{

    S.erase(std::remove(
                S.begin(), S.end(), *c),
            S.end());

    return S;
}

int SDP::Parse(std::string sdpString)
{
    fullSDP = sdpString;

    std::vector<std::string> lines;

    boost::split(lines, sdpString, boost::is_any_of("\n"));

    KeyType lastKey = KeyType::None;
    for(auto line: lines) {

        line = removeCharacters(line , "\r");

        std::vector<std::string> key_value;

        std::string key;
        std::string value;


        boost::split(key_value, line, boost::is_any_of("="));

        if(key_value.size()  <= 1) continue;
        key = key_value.at(0);
        value  = key_value.at(1);




        switch(resolve_key_type(key)) {


            case KeyType::Version:
                this->version = std::stoi(value);
                lastKey = KeyType::Version;
                break;


            case KeyType::Origin:
                this->origin = value;
                lastKey = KeyType::Origin;
                break;


            case KeyType::Session:
                this->session = value;
                lastKey = KeyType::Session;
                break;


            case KeyType::Timing:
                this->timing = value;
                lastKey = KeyType::Timing;
                break;


            case KeyType::Media: {
                //std::cout << "Media found" << std::endl;
                MediaDescription media(value);
                this->mediaDescriptionList.push_back(media);
                lastKey = KeyType::Media;
                break;
            }
            case KeyType::Connection: {

                if(lastKey != KeyType::None && lastKey == KeyType::Media) {
                    mediaDescriptionList.back().c = value;
                }
            }

            case KeyType::Attribute: {
                //std::cout << ("Attribute found.") << std::endl;
                //std::cout << "lastKey " << lastKey << std::endl;
                //std::cout << "key is " << key << std::endl;
                if(boost::starts_with(value, "group:BUNDLE")) {
                    //std::cout<<"STARTS WITH BUNDEL???????????????" << std::endl;
                    std::vector<mid> mids;
                    boost::split(mids, value, boost::is_any_of(" "));
                    mids.erase(mids.begin());
                    for(auto singleMid : mids ) {
                        bundle.push_back(singleMid);

                    }
                }
                else if(boost::starts_with(key, "msid-bundle")) {


                }
                else if(lastKey == KeyType::Media) {
                    mediaDescriptionList.back().AddAttribute(value);
                }
                break;
            }
            default:
                CONSOLELOG("Unknown type of SDP key provided.");
                break;
        }

    }
    return 0;

}


int SDP::resolve_key_type(std::string key) {

    if(key == "v") return KeyType::Version;
    if(key == "o") return KeyType::Origin;
    if(key == "s") return KeyType::Session;
    if(key == "t") return KeyType::Timing;
    if(key == "m") return KeyType::Media;
    if(key == "a") return KeyType::Attribute;
    if(key == "c") return KeyType::Connection;
    return -1;
}

