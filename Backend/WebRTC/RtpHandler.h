#ifndef RTPHANDLER_H_INCLUDED
#define RTPHANDLER_H_INCLUDED

#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>

#ifdef _WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#endif

#include "Sdp.h"
#include "RTPPacket.h"
//#include <unistd.h>
#include <string.h>


#include <srtp.h>
#include <mutex>
#include <future>

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <opus.h>
#include "common_audio/vad/include/webrtc_vad.h"

#include <list>



#define SRTP_MASTER_KEY_KEY_LEN 16
#define SRTP_MASTER_KEY_SALT_LEN 14

// typedef enum Codec Codec;
typedef boost::shared_mutex Lock;
typedef boost::unique_lock< Lock >  WriteLock;
typedef boost::shared_lock< Lock >  ReadLock;

class PeerConnection;


class VAD {
public:
    VAD(Codec codec, int audioSampleRate, int channels);
    ~VAD() = default;
    
    void StartOpusWorker();
    void StartWorker();
    //void DecodeVoice(unsigned char* Data, int DataLen);
    void DecodeVoice();
    bool CurrentlySpeaking = false;

    int SampleRate = 0;
    int Channels = 1;
    Codec codec;
    VadInst* vadModule;
    bool NewDataAvailable = false;

    //unsigned char* Data;
    //int DataLen=0;
    std::shared_ptr<opus_int16> DecodedData;
    unsigned char Data[1500];
    int DataLen = 0;
    int DecodedDataLen=0;

    OpusDecoder* Decoder;
    static std::mutex DecoderMutex;


    bool shutdownVAD = false;
    bool shutdownComplete = false;
    void Shutdown() {
        shutdownVAD = true;
        waitForData.notify_one();
        while (!shutdownComplete) {
        
        
        }
    }

    boost::mutex notifyLock;
    boost::condition_variable waitForData;

};


class RTPHandler {
public:
    RTPHandler(std::vector<unsigned int> ssrc, std::string mid, std::string msid, std::string cname, std::string iceuser, std::string icepass);
    ~RTPHandler();
    void Listen(std::string host, int port);
    void AddMediaDescription(MediaDescription);

    void SetSenderMode(bool);
    void SetSendSource(std::shared_ptr<RTPHandler>);
    //void ForwardPackets(int sock, struct sockaddr_in a,  socklen_t l);
    void ForwardPackets(unsigned char* packetdata, int packetdataLen, bool rtcp);
    void UnProtect(RTPPacket* p);
    void SignalReaders(unsigned char* dataToSend, int _packetLen, int type);
    //void SignalReaders();

    void PutData();
    void Shutdown();
    bool CanSendRTCP(RTPHandler*);

    void init_openssl();
    void configure_context(SSL_CTX*);

    int create_socket(std::string address, int port);
    void SockInit();
    void SockClose();
    std::vector<unsigned int> ssrc;
    std::string mid;
    std::string msid;
    std::string cname;
    std::string iceuser;
    std::string icepass;


    std::mutex readersMutex;
    std::vector<RTPHandler*> readers;



    static int (generatecookie) (SSL *ssl, unsigned char *cookie, unsigned int *cookie_len);
    static int (verifycookie) (SSL *ssl, const unsigned char *cookie, unsigned int cookie_len);
    //static unsigned char cookie[4];

    unsigned char sourceData[1500];
    int sourceDataLen = 0;
    int dataType = 0;

    boost::mutex notifyLock;
    boost::condition_variable waitForData;


    //unsigned char dataToSend[8000];
    //int _packetLen;
    //int typ;
    //bool processingData = false;
    //boost::condition_variable waitForDataFromSource;
    //boost::mutex notifyDataFromSource;

    std::vector<RTPPacket> packets;

    bool SenderMode = false;
    bool NewDataAvailable = false;
    bool readyToReceive = false;

    bool shutdownRtp = false;
    bool receiverThreadShutdown = false;
    bool senderThreadShutdown = false;


    std::shared_ptr<RTPHandler> source;


    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int sock;

    std::mutex packetsLock;

    SSL *ssl;
    SSL_CTX *ctx;
    srtp_t srtpSession;
    srtp_t srtpSession2;
    BIO* rbio;
    BIO* wbio;

    bool isAudioStream = false;
    Codec audioCodec;
    int audioSampleRate = 0;
    int channels = 1;
    std::shared_ptr<VAD> vad;


};

#endif // RTPHANDLER_H_INCLUDED
