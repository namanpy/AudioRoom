#include "Ice.h"
#include <curl/curl.h>
#include <cstring>
#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#endif
#include <cstring>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <cmath>
#include <limits>
ICECandidate::ICECandidate(double foundation, int componentId, std::string transport, double priority, std::string connectionAddress, int port, std::string candidateType)
{
    this->foundation = foundation;
    this->componentId = componentId;
    this->transport = transport;
    this->priority = priority;
    this->connectionAddress = connectionAddress;
    this->port = port;
    this->candidateType = candidateType;
}
std::string generateRandomNum(int length) {

    std::string randomString;

    std::string chars(
//         "abcdefghijklmnopqrstuvwxyz"
//         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
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


std::vector<std::string> _splitFirstOf(std::string s, std::string c) {

    std::vector<std::string> ls;
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
    return ls;

}

std::string ICECandidate::ToString()
{
    std::string foundationStr = _splitFirstOf(std::to_string(foundation), ".")[0];
    std::string candidate = "candidate:" + foundationStr +\
                            " " +\
                            _splitFirstOf(std::to_string(componentId), ".")[0] +\
                            " " +\
                            (transport) +\
                            " " +\
                            _splitFirstOf(std::to_string(priority), ".")[0] +\
                            " " +\
                            (connectionAddress) +\
                            " " +\
                            std::to_string(port) +\
                            " typ " +\
                            (candidateType) +\
                            "";
//                             "" +
                      //      + " generation 0";
    return candidate;
}



double getPriority(bool ipv6, int componentId) {
    if(!ipv6){
        return pow(2, 24) * (126) + pow(2, 8) * (65535) + pow(2, 0) * (256 - 1);
    } else {
        //TODO IPv6 some day
    }
    return 0;

}

ICECandidate ICE::GenerateCandidates(bool rtcpMux)
{

    //std::cout << "GENERATING RANDOM NO" << std::endl;

    srand(stoi(generateRandomNum(6)));
    std::vector<ICECandidate*> candidates;

    std::string connectionAddress;
    //std::cout << "Find Local Ip" << std::endl;
#ifdef WEBRTC_DEBUG
    FindLocalIP(connectionAddress);
#else
   // FindLocalIP(connectionAddress);

    FindPublicIP(connectionAddress);
#endif
    //std::cout << "End locl ip" << std::endl;
    double foundation = stod(generateRandomNum(8));
    int componentId = 1;
    std::string transport = "udp";
    double priority = getPriority(false, componentId);

    int port = rand()%(25000-60000+1)+25000;
    std::string candidateType = "host";

    ICECandidate c1 =  ICECandidate(foundation, componentId, transport, priority, connectionAddress, port, candidateType);

    if(!rtcpMux) {
        //TODO
    }


//     if(rtcpMux) {
//         componentId = 2;
//         priority = getPriority(false, componentId);
//         ICECandidate* c2 = new ICECandidate(foundation, componentId, transport, priority, connectionAddress, port, candidateType);
//
//         candidates.push_back(c1);
//         candidates.push_back(c2);
//
//         return candidates;
//     } else {
//         //TODO
//     }

    /*
    candidates.push_back(*/
    //std::cout << "Function endded!!!!!!!!!! " << std::endl;
    return c1;
}

size_t callback_func(void* ptr, size_t size/*always==1*/,
                         size_t nmemb, void* userdata)
{
    size_t dataSize=0;
    char** stringToWrite=(char**)userdata;
    const char* input=(const char*)ptr;
    if(nmemb==0) return 0;

    *stringToWrite= (char *) malloc(nmemb+1);

    memcpy(*stringToWrite+dataSize, input, nmemb);
    dataSize+=nmemb;
    (*stringToWrite)[dataSize]='\0';
    return nmemb;
}

void ICE::FindPublicIP(std::string &ip) {

  CURL *curl;
  CURLcode res;
  char* str;
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org/?format=text");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_func);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_easy_cleanup(curl);
    ip = std::string(str); //TODO sus

    }


}


void ICE::FindLocalIP(std::string &ip) {
    const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;

#ifdef _WIN32
    WSADATA wsa_data;
    int e = WSAStartup(MAKEWORD(1, 1), &wsa_data);
    //std::cout << "ICE-WSA Startup >> " << e << std::endl;
#endif
    struct sockaddr_in serv;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    //Socket could not be created
    if(sock < 0)
    {
        //std::cout << "Socket error" << std::endl;
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
   inet_pton(AF_INET, google_dns_server, &serv.sin_addr.s_addr);
    serv.sin_port = htons(dns_port);

    int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));
    if (err < 0)
    {
        //std::cout << "Error number: " << errno << ". Error message: " << "n.a" << std::endl;
    }

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*)&name, &namelen);

    char buffer[80];
    const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
    if(p != NULL)
    {
       ip = buffer;
    }
    else
    {
        //std::cout << "Error number: " << errno << ". Error message: " << "n.a" << std::endl;
    }

#ifdef _WIN32
    
    WSACleanup();
#else
    close(sock);
    return;
#endif

}
