#ifndef SDP_H_INCLUDED
#define SDP_H_INCLUDED

#include <iostream>
#include "boost/algorithm/string.hpp"
#include <map>
#include <vector>

typedef std::string Key;
typedef std::string Value;
typedef std::string mid;

enum KeyType {
    Version,
    Origin,
    Session,
    Timing,
    Media,
    Attribute,
    Connection,
    None
};

enum Codec {
    Opus,
    Other
};


class MediaDescription {
public:

    MediaDescription(std::string value);
    int AddAttribute(std::string attribute);

    std::vector<std::string> extensions;
    std::vector<std::string> acceptedCodecs;
    std::map<std::string, std::vector<std::string>> codecInfo;
    std::map<std::string, std::vector<std::string>>  feedbackInfo;
    std::vector<unsigned int> ssrc;

    std::string fingerprintEncryption;
    std::string fingerprint;
    std::string mid;
    std::string msid;
    std::string cname;
    std::string mediaType;
    std::string transportType;
    std::string ice_ufrag;
    std::string ice_pwd;
    std::string port;
    std::string c;
    bool rtcp_mux;
    std::string rtcp_rsize;

    bool audio = false;
    int sampleRate = 0;
    int channels = 0;
    Codec audioCodec;

};

class SDP {
public:
    int Parse(std::string sdpString);
    std::string ToString();

// private:
    int version;
    std::string fullSDP;
    std::string origin;
    std::string session;
    std::string timing;
    std::string connection;
    std::vector<mid> bundle;
    std::vector<MediaDescription> mediaDescriptionList;
    int resolve_key_type(std::string);


};

#endif // SDP_H_INCLUDED
