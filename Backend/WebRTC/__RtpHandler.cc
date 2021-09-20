#include <stdio.h>
#include <string.h>
#define _CRTDBG_MAP_ALLOC 1
#include <stdlib.h>
#include <crtdbg.h>
#include "RtpHandler.h"
#include "stun.h"
#include <thread>
#include <fstream>
#include "common_audio/vad/include/webrtc_vad.h"
#include <boost/algorithm/string.hpp>
#include <chrono>


#ifdef _WIN32
#include <direct.h>
#define MSG_DONTWAIT 0
#else
#include <unistd.h>
#endif


#include <boost/asio/detail/socket_ops.hpp>
#ifdef _DEBUG
#define DBG_NEW new
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


#define _CRT_SECURE_NO_WARNINGS 
#define SRTP_MASTER_KEY_KEY_LEN 16
#define SRTP_MASTER_KEY_SALT_LEN 14



std::string GetCWD() {
    char buf[5000];
#ifdef _WIN32
    _getcwd(buf, 5000);
#else
    getcwd(buf, 5000);
#endif

    std::string cwd(buf);

    return cwd;
}


VAD::VAD(Codec codec, int audioSampleRate, int channels) {
    this->codec = codec;
    this->SampleRate = audioSampleRate;
    this->Channels = channels;
    if(codec == Codec::Opus) {
        std::thread(&VAD::StartOpusWorker, this).detach();
    }

}

void VAD::StartOpusWorker() {

    int error = -69;
    opus_int16 DecodedData[8000];
    int DecodedDataLen = 0;
    VadInst* vadModule;

    vadModule = WebRtcVad_Create();
    int vaderr = WebRtcVad_Init(vadModule);
    WebRtcVad_set_mode(vadModule, 3);

    if(vaderr == -1){
        std::cout << "Failed to intialize VAD" << std::endl;
        exit(0);
    }

    Decoder = opus_decoder_create(SampleRate, Channels, &error);

    if(error == OPUS_OK) {

    } else {
        std::cout << "Opus decoder err " << error << std::endl;
        exit(0);
    }

    while(true) {
        if (this->shutdownVAD == true) {
            
            std::cout << "VAD::StartOpusWorker  >> Shutting down VAD Module " << std::endl;
               
            opus_decoder_destroy(Decoder);
            WebRtcVad_Free(vadModule);
            shutdownComplete = true;
            return;
        }

        if(NewDataAvailable) {

            int sample =  opus_decoder_get_nb_samples ( Decoder, Data, DataLen );
            //int frame_size = opus_packet_get_nb_frames(Data, DataLen);
            if(sample == OPUS_BAD_ARG || sample == OPUS_INVALID_PACKET) {

                //std::cout << "Couldn't get number of frames " << std::endl;
                exit(0);
            }



            int frame_size = opus_decode(Decoder, Data, DataLen, DecodedData, sample *  Channels, 0);

            DecodedDataLen = frame_size * Channels * sizeof(opus_int16);

            if(frame_size >0) {
                //WebRtcVad_ValidRateAndFrameLength(int rate, size_t frame_length)
                int voiceDetected = WebRtcVad_Process(vadModule, SampleRate, DecodedData, frame_size);

                if(voiceDetected == 0) {

                    CurrentlySpeaking = false;

                } else if (voiceDetected > 0) {
                    CurrentlySpeaking = true;
                    //std::cout << "speaking" << std::endl;
                }

            } else {
                //std::cout << "Error! " << frame_size << std::endl;
            }

            NewDataAvailable=false;
        } else {

            boost::unique_lock<boost::mutex> lk(notifyLock);
            waitForData.wait(lk);

        }

    }


}





unsigned char cookie[4] = {0x00, 0x00, 0x00, 0x00};

int RTPHandler::generatecookie(SSL *ssl, unsigned char *cookie_, unsigned int *cookie_len) {

    return 1;

}

int RTPHandler::verifycookie(SSL* ssl, const unsigned char* cookie_, unsigned int cookie_len)
{


    return 1;
}

int RTPHandler::create_socket(std::string address, int port)
{
    int s;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, address.c_str(), &addr.sin_addr.s_addr);
    //std::cout << "Trying to bind " << address << " , " << port << std::endl;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
	perror("Unable to create socket");
	exit(EXIT_FAILURE);
    }

    int status = bind(s, (struct sockaddr*)&addr, sizeof(addr));
    if (status < 0) {
        //std::cout << "Bind ? "  << status << " "  << WSAGetLastError() << std::endl;
	perror("Unable to bind????????");
	exit(EXIT_FAILURE);
    }

//     if (listen(s, 1) < 0) {
// 	perror("Unable to listen");
// 	exit(EXIT_FAILURE);
//     }

    return s;
}

void RTPHandler::init_openssl()
{
    SSL_load_error_strings();
  //  SSL_library_init();
    //std::cout << "Init SSL" << std::endl;
   OpenSSL_add_ssl_algorithms();
   //std::cout << "SSL initialized " << std::endl;
}

void cleanup_openssl()
{
    EVP_cleanup();
}

SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = DTLS_server_method();

    ctx = SSL_CTX_new(method);
//     SSL_CTX_use_certificate_file(ctx, "", SSL_FILETYPE_PEM);
//     SSL_CTX_use_PrivateKey_file(ctx, "", SSL_FILETYPE_PEM);
    if (!ctx) {
	perror("Unable to create SSL context");
	ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }

    return ctx;
}

void RTPHandler::configure_context(SSL_CTX *ctx)
{
    //const char* dir;

    //dir = getenv(x509_get_default_cert_dir_env());

    //if (!dir)
    //    dir = x509_get_default_cert_dir();
    //puts(dir);
//    SSL_CTX_set_ecdh_auto(ctx, 1);

    /* Set the key and cert */
    //SSL_CTX_set_cookie_generate_cb(ctx, &RTPHandler::generatecookie);
    //SSL_CTX_set_cookie_verify_cb(ctx, &RTPHandler::verifycookie);
//    SSL_CTX_set_min_proto_version(ctx, DTLS1_2_VERSION);

    //C:/Users/naman/Desktop/Development/AudioRoomVS/AudioRoom/x64/Debug

  
    std::string certlocation = GetCWD() + std::string("/cert/cert.pem");
    std::string keylocation = GetCWD() + std::string("/cert/key.pem");


    boost::algorithm::replace_all(certlocation, ("\\"), ("/"));
    boost::algorithm::replace_all(keylocation, ("\\"), ("/"));

    std::cout << "certlocation : " << certlocation << std::endl;
    if (SSL_CTX_use_certificate_file(ctx, certlocation.c_str() , SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
	    exit(0);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, keylocation.c_str(), SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
	    exit(0);
   }




}


RTPHandler::RTPHandler(std::vector<unsigned int> ssrc, std::string mid, std::string msid, std::string cname, std::string iceuser, std::string icepass)
{

    ////std::cout << "ssrc is " << ssrc[0] << " mid is " << mid << " msid " << msid << "cname " << cname << " iceuser " << iceuser << " icepass " << icepass;

    this->ssrc = ssrc;
    this->mid = mid;
    this->msid = msid;
    this->cname = cname;
    this->iceuser = iceuser;
    this->icepass = icepass;


}





void RTPHandler::SetSenderMode(bool val) {

    SenderMode = val;

}

void RTPHandler::SetSendSource(std::shared_ptr<RTPHandler> r)
{

    source = r;

}

int CharToInt(unsigned char* data, int start, int len) {
    int value = 0;

    for(int i = start; i < start + len; i++) {
//             //std::cout << "ree value " << data[i] << std::endl;
        value = (value << (sizeof(data[i]) * 8)) | data[i];
    }
    return value;
}



void RTPHandler::ForwardPackets(int sock, struct sockaddr_in a,  socklen_t l)
{

    packets.reserve(256);

    struct sockaddr_in addr = a;
    socklen_t len = l;



    bool firstPacket = true;
    unsigned int lastTimestamp = 0;
    while(true) {
        if(shutdownRtp) {
            //std::cout << "[shutdown] Got shutdown msg" << std::endl;
            senderThreadShutdown = true;
            break;
        }
        if (SenderMode) {
            //std::cout << "Sender Mode Packet Size()" << packets.size() << std::endl;
        }

        if(packets.size() >= 1) {
            //std::cout << "Got a packet to send." << std::endl;

//             unsigned int currentTimestamp = CharToInt(packets.front().data, 4, 4);

            if(firstPacket) {
                if(packets.front().rtcp) {


                    int err = srtp_protect_rtcp(srtpSession2, &packets.front().data , &packets.front().packetLen);
                    if(err == 0) {
                        sendto(sock, (const char*) packets.front().data, packets.front().packetLen, MSG_DONTWAIT,(struct sockaddr *) &addr, len);

                    } else {

                        sendto(sock, (const char*) packets.front().data, packets.front().packetLen, MSG_DONTWAIT,(struct sockaddr *) &addr, len);

                    }




                } else {

                    int err = srtp_protect(srtpSession2, &packets.front().data , &packets.front().packetLen);
                    if(err == 0) {

                        sendto(sock, (const char*)packets.front().data, packets.front().packetLen, MSG_DONTWAIT,(struct sockaddr *) &addr, len);

                    }
                    firstPacket = false;

                }
               WriteLock wlock(lock);
               packets.erase(packets.begin());
               wlock.unlock();
            } else {

                if(packets.front().rtcp) {

                        int err = srtp_protect_rtcp(srtpSession2, &packets.front().data , &packets.front().packetLen);
                        if(err == 0) {
                            sendto(sock, (const char*) packets.front().data, packets.front().packetLen, MSG_DONTWAIT,(struct sockaddr *) &addr, len);

                        } else {
                            sendto(sock, (const char*) packets.front().data, packets.front().packetLen, MSG_DONTWAIT,(struct sockaddr *) &addr, len);
                            std::cout << "Error RTCP from remote stream Error: " << err << std::endl;

                        }

                } else {//

                    int err = srtp_protect(srtpSession2, &packets.front().data , &packets.front().packetLen);
                    if(err == 0) {
                        //std::cout << "Sending data"  << err << std::endl;
                        sendto(sock, (const char*) packets.front().data, packets.front().packetLen, MSG_DONTWAIT,(struct sockaddr *) &addr, len);

                    }
                    else {
                    
                        std::cout << "Error encrypting video from remote stream Error: " << err << std::endl;
                    
                    }


                }
                WriteLock wlock(lock);
                packets.erase(packets.begin());
                wlock.unlock();

            }

        }else {

                   boost::unique_lock<boost::mutex> lk(notifyLock);
                   waitForData.wait(lk);

        }

    }

}






void RTPHandler::Listen(std::string address, int  port)
{
    try {
        //std::cout << "LISTEN STARTED REE" << std::endl;
        SockInit();
        //std::cout << "Sock init done" << std::endl;
        init_openssl();
        //std::cout << "ssl init done " << std::endl;

        //std::cout << "LISTEN SENDED" << std::endl;
        ctx = create_context();

        configure_context(ctx);

        ssl = SSL_new(ctx);

        rbio = BIO_new(BIO_s_mem());
        wbio = BIO_new(BIO_s_mem());

        SSL_set_accept_state(ssl);
        SSL_set_bio(ssl, rbio, wbio);


        if (SSL_set_tlsext_use_srtp(ssl, "SRTP_AES128_CM_SHA1_80") != 0) {
            ERR_print_errors_fp(stderr);
            exit(EXIT_FAILURE);
        }

        stack_st_SRTP_PROTECTION_PROFILE* profile = SSL_get_srtp_profiles(ssl);

        if (profile == NULL) {
            //std::cout << "No Protection profile configured" << std::endl;
        }
        //ReadLock readLock(lock);
    //     notifyLock = boost::unique_lock<boost::shared_mutex>(notifyLockMutex);
    //     notifyLock.unlock();

        //std::cout << "Creating socket" << std::endl;



        sock = create_socket(address, port);

        //SRTP
        srtp_init();

        srtp_policy_t policy{};
        unsigned char dtls_buffer[SRTP_MASTER_KEY_KEY_LEN * 2 + SRTP_MASTER_KEY_SALT_LEN * 2];
        unsigned char client_write_key[SRTP_MASTER_KEY_KEY_LEN + SRTP_MASTER_KEY_SALT_LEN];
        unsigned char server_write_key[SRTP_MASTER_KEY_KEY_LEN + SRTP_MASTER_KEY_SALT_LEN];
        size_t offset = 0;
        const char* label = "EXTRACTOR-dtls_srtp";


        bool doHandshake = false;
        bool isSrtpSetup = false;

        unsigned int packetId = 0;

        //std::ofstream MyFile("/home/naman/Desktop/Call Server/MediaDevices/h264");

       // FILE* file = popen( "ffmpeg -i -  'home/naman/Desktop/Call Server/MediaDevices/test2.mp4'", "w" );
        //FILE* file = fopen( "/home/naman/Desktop/Call Server/MediaDevices/a", "w" );
        const char reply[] = "test\n";
        unsigned char buffer[8000];
        int len_ = 8000;
        while (1) {
           
            if (shutdownRtp) {

                break;
            }


            int packetLen = recvfrom(sock, (char*)buffer, len_, 0, (struct sockaddr*)&addr, &len);
            if (packetLen <= 0) {
                perror( std::string("RTPHandler::Listen() Unable to accept " + iceuser).c_str());
                
            }

            if (STUN::IsStun(buffer)) {
                //std::cout << "packet is stun!" << std::this_thread::get_id() << std::endl;

                STUN s = STUN();
                STUN p;
                s.Parse(buffer, packetLen);
                unsigned char* key = (unsigned char*)icepass.c_str();
                p.Init(STUN::StunType::BINDING_RESPONSE, s.transactionId);
                p.AddXORMappedAddress(addr.sin_addr.s_addr, addr.sin_port);
                p.AddMessageIntegrity(key, icepass.length());
                p.AddFingerprint();

                sendto(sock, (const char*)p.data_, p.len, MSG_DONTWAIT, (struct sockaddr*)&addr, len);





            }
            else if ((buffer[0] >= 20) && (buffer[0] <= 64)) {
                if (doHandshake == true) {

                    //std::cout << std::endl;
                    for (int j = 0; j < packetLen; j++) {
                        //std::cout << std::hex << buffer[j];
                    }

                    //std::cout << std::endl;

                }
                int i = 0;
                int written = BIO_write(rbio, buffer, packetLen);

                //                 while(true) {m
                i++;
                int res = SSL_accept(ssl);

                if (res >= 1) {
                    //std::thread(&RTPHandler::SignalReaders, this).detach();
                    std::cout << "Handkshake done" << std::endl;
                    int dataToWrite = BIO_read(wbio, buffer, len_);
                    if (dataToWrite == 0 || dataToWrite == -1) {
                        //std::cout << "" << std::endl;
                    }
                    else {
                        //std::cout << i << " Have some data to write" << dataToWrite << std::endl;
                        sendto(sock, (const char*)buffer, dataToWrite, MSG_DONTWAIT, (struct sockaddr*)&addr, len);
                    }

                    if (doHandshake) continue;

                    int exportStatus = SSL_export_keying_material(ssl,
                        dtls_buffer,
                        sizeof(dtls_buffer),
                        label,
                        strlen(label),
                        NULL,
                        0,
                        0);

                    if (exportStatus >= 1) {
                        //std::cout << "Key Export Success." << std::endl;
                        //std::cout << SRTP_MASTER_KEY_KEY_LEN << "is key len" << std::endl;
                        memcpy(&client_write_key[0], &dtls_buffer[offset], SRTP_MASTER_KEY_KEY_LEN);
                        offset += SRTP_MASTER_KEY_KEY_LEN;
                        memcpy(&server_write_key[0], &dtls_buffer[offset], SRTP_MASTER_KEY_KEY_LEN);
                        offset += SRTP_MASTER_KEY_KEY_LEN;
                        memcpy(&client_write_key[SRTP_MASTER_KEY_KEY_LEN], &dtls_buffer[offset], SRTP_MASTER_KEY_SALT_LEN);
                        offset += SRTP_MASTER_KEY_SALT_LEN;
                        memcpy(&server_write_key[SRTP_MASTER_KEY_KEY_LEN], &dtls_buffer[offset], SRTP_MASTER_KEY_SALT_LEN);

                       // srtp_crypto_policy_set_rtp_default(&policy.rtp);
                        //srtp_crypto_policy_set_rtcp_default(&policy.rtcp);
                        srtp_crypto_policy_set_aes_cm_128_hmac_sha1_80(&policy.rtp);
                        srtp_crypto_policy_set_aes_cm_128_hmac_sha1_80(&policy.rtcp);

                        if (SenderMode) {
                            
                            policy.ssrc.value = 0;
                            if (ssrc.size() > 0) {
                                policy.ssrc.value = ssrc[0];
                            }
                            policy.ssrc.type = ssrc_any_outbound;
                            policy.next = NULL;
                            policy.key = server_write_key;
                            int err = srtp_create(&this->srtpSession2, &policy);
                            if (err == 0) {
                                //std::cout << "Created SRTP session.";
                                isSrtpSetup = true;

                            }
                            else {
                                //std::cout << "SRTP Session couldn't be created. " << err << std::endl;
                            }
                            policy.ssrc.value = 0;
                            if (ssrc.size() > 0) {
                                policy.ssrc.value = ssrc[0];
                            }
                            policy.ssrc.type = ssrc_any_inbound;
                            policy.next = NULL;
                            policy.key = client_write_key;
                            int err2 = srtp_create(&this->srtpSession, &policy);
                            if (err2 == 0) {
                                //std::cout << "Created SRTP session.";
                                isSrtpSetup = true;
                                //                                     if(SenderMode) {
                                //                                         ForwardPackets(sock);
                                //                                     }
                            }
                            else {
                                //std::cout << "SRTP Session couldn't be created. " << err << std::endl;
                            }
                            readyToReceive = true;

                            //std::cout << "exiting sender mode" << std::endl;
                            
                            std::thread(&RTPHandler::ForwardPackets, this, sock, addr, len).detach();



                        }
                        else {
                            policy.ssrc.value = ssrc[0];
                            policy.ssrc.type = ssrc_any_inbound;
                            policy.next = NULL;
                            policy.key = client_write_key;
                            int err = srtp_create(&srtpSession, &policy);
                            if (err == 0) {
                                //std::cout << "Created SRTP session.";
                                isSrtpSetup = true;

                                if (isAudioStream) {
                                    //std::cout << "STARTING AUDIO STREAM WITH CHANNELS >> " << channels << std::endl;
                                    vad = std::unique_ptr<VAD>(DBG_NEW VAD(audioCodec, audioSampleRate, channels));
                                }
                            }
                            else {
                                //std::cout << "SRTP Session couldn't be created. " << err << std::endl;
                            }

                            policy.ssrc.value = ssrc[0];
                            policy.ssrc.type = ssrc_any_outbound;
                            policy.next = NULL;
                            policy.key = server_write_key;
                            int err2 = srtp_create(&srtpSession2, &policy);
                            if (err2 == 0) {
                                //std::cout << "Created SRTP session for Feedback ";
                                isSrtpSetup = true;

                            }
                            else {
                                //std::cout << "SRTP Session couldn't be created. " << err << std::endl;
                            }

                            //                                 std::thread(&RTPHandler::SignalReaders, this).detach();
                            readyToReceive = true;
                            std::thread(&RTPHandler::ForwardPackets, this, sock, addr, len).detach();

                        }

                    }
                    else {
                        //std::cout << "Key Export FAILED." << std::endl;

                    }
                    doHandshake = true;
                }
                else {

                    int dataToWrite = BIO_read(wbio, buffer, len_);
                    if (dataToWrite == 0 || dataToWrite == -1) {
                        //std::cout << "" << std::endl;
                    }
                    else {
                        //std::cout << i << " Have some data to write" << dataToWrite << std::endl;
                        sendto(sock, (const char*)buffer, dataToWrite, MSG_DONTWAIT, (struct sockaddr*)&addr, len);
                    }

                    int dtlsError = SSL_get_error(ssl, res);
                    if (dtlsError == SSL_ERROR_WANT_READ) {
                        std::cout << i << " SSL ERROR WANT READ" << std::endl;
                    }
                    else   if (dtlsError == SSL_ERROR_WANT_WRITE) {
                        std::cout << i << " SSL ERROR WANT WRITE" << std::endl;
                    }
                }




            }
            else {
                if (isSrtpSetup) {
                   // if (!processingData) {
                        if (buffer[1] == 200 || buffer[1] == 201 || buffer[1] == 204 || buffer[1] == 205) {


                            SignalReaders(buffer, packetLen, 1);
            /*                _packetLen = packetLen;
                            memcpy(dataToSend, buffer, packetLen);
                            typ = 1;
                            waitForDataFromSource.notify_one();*/

                        }
                        else {
          /*                  _packetLen = packetLen;
                            memcpy(dataToSend, buffer, packetLen);
                            typ = 0;
                            waitForDataFromSource.notify_one();*/

                             SignalReaders(buffer, packetLen, 0);



                        }
                    //}
                    //else {
                    //    //std::cout << "Can't keep up , skipping " << std::endl;
                    //}
                }
            }



        }
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        cleanup_openssl();
        receiverThreadShutdown = true;

    }
    catch (std::exception e) {
        //std::cout << "Error RTP Handler : " << e.what() << std::endl;
    }
}



void RTPHandler::UnProtect(RTPPacket* p)
{

    int err = srtp_unprotect(srtpSession, &p->data , (int*) &p->packetLen);

    if(err==0) {
        p->unprotected = true;
    }

}

bool RTPHandler::CanSendRTCP(RTPHandler* rh) {
    std::unique_lock<std::mutex> l(readersMutex);
    if (readers.size() > 0) {
        if (readers[0] == rh) {
            return true;

        }
    }
    else {
        
        return true;
        
    }

    return false;
}
//unsigned char* dataToSend, int _packetLen, int typ
void RTPHandler::SignalReaders(unsigned char* dataToSend, int _packetLen, int typ)
{

    //while (true) {
        //processingData = false;
        //boost::unique_lock<boost::mutex> lck(notifyDataFromSource);
        //waitForDataFromSource.wait(lck);
        //processingData = true;
        int err = -69;
        if (typ == 0) {
            err = srtp_unprotect(srtpSession, dataToSend, &_packetLen);
        }
        else {
            err = srtp_unprotect_rtcp(srtpSession, dataToSend, &_packetLen);
        }
        if (err == 0) {


            if (SenderMode) {

                RTPPacket packet;
                packet.id = 0;
                if (typ == 1) {
                    packet.rtcp = true;
                }

                memcpy(&packet.data, dataToSend, _packetLen);
                packet.packetLen = _packetLen;


                if (source == nullptr) {

                    return;

                }
                //if (source->CanSendRTCP(this)) {
                    WriteLock wlock(source->lock);
                    source->packets.push_back(packet);
                    wlock.unlock();
                    //boost::unique_lock<boost::mutex> lck(source->notifyLock);
                    source->waitForData.notify_one();
                /*}*/


                return;
            }




            std::unique_lock<std::mutex> l(readersMutex);

            for (auto reader : readers) {


                RTPPacket packet{};
                packet.id = 0;
                memcpy(&packet.data, dataToSend, _packetLen);
                packet.packetLen = _packetLen;
                if (typ == 1) {
                    packet.rtcp = true;
                }


                WriteLock wlock(reader->lock, boost::try_to_lock);
                if (wlock) {

                    if (reader->packets.size() > 12) {

                        std::cout << "cant keep up skipping" << std::endl;
                        return;
                    }
                    reader->packets.push_back(packet);
                    //std::cout << "size() " << reader->packets.size() << std::endl;
                    wlock.unlock();
                    //boost::unique_lock<boost::mutex> lk(reader->notifyLock);
                    reader->waitForData.notify_one();
                }



            }
            if (isAudioStream) {
                if (!vad->NewDataAvailable && typ == 0) {
                    memcpy(vad->Data, dataToSend + 28, _packetLen - 28);
                    vad->DataLen = _packetLen - 28;
                    vad->NewDataAvailable = true;
                    // boost::unique_lock<boost::mutex> lk(vad->notifyLock);
                    vad->waitForData.notify_one();
                }
            }
        }
        else {
            //std::cout << "Could not decode packet typ: " << typ << " err: " << err;
            //std::cout << "ssrc is " << ssrc[0] << "  buffer[1]" << (int) dataToSend[1] << std::endl;

        }


    //}

}


void RTPHandler::Shutdown() {

    //if (SenderMode) {
    //    


    if (source != nullptr) {
        std::unique_lock<std::mutex> l(source->readersMutex);
        int i = 0;
        for (auto r : source->readers) {

            if (r == this) {

                source->readers.erase(source->readers.begin() + i);
            }
            i++;
        }
    }
    //}

    SockClose();
    shutdownRtp = true;


    waitForData.notify_one();

    //boost::unique_lock<boost::mutex> lck(notifyLock);

    while (!receiverThreadShutdown || !senderThreadShutdown) {

       // std::cout << "Waiting to shutdown threads...." << senderThreadShutdown << " - " << receiverThreadShutdown << std::endl;
        waitForData.notify_one();


    }

    if (isAudioStream) {
        vad->Shutdown();
    }
}

void  RTPHandler::SockInit(void)
{
#ifdef _WIN32
    WSADATA wsa_data;
    int e = WSAStartup(MAKEWORD(1, 1), &wsa_data);

    //std::cout << "WSA Startup >> " << e << std::endl;
#else
    return 0;
#endif
}

void RTPHandler::SockClose(void)
{
#ifdef _WIN32
    closesocket(sock);
    shutdown(sock, 2);
#else
    shutdown(sock, SHUT_RDWR);
#endif
}



void RTPHandler::PutData( )
{
        if(!readyToReceive) return;

//            if(NewDataAvailable == false) {
//                 memcpy(&sourceData, _data, _len);
//                 sourceDataLen = _len;
//                 dataType = typ;
//                 NewDataAvailable = true; }

                if(packets.front().rtcp) {

                    int err = srtp_protect_rtcp(srtpSession2, &packets.front().data , &packets.front().packetLen);
                    if(err == 0) {
                        sendto(sock, (const char*) packets.front().data, packets.front().packetLen, MSG_DONTWAIT,(struct sockaddr *) &addr, len);

                    } else {
                        sendto(sock, (const char*)packets.front().data, packets.front().packetLen, MSG_DONTWAIT,(struct sockaddr *) &addr, len);

                    }

                } else {//
                    //std::cout << "Forwarding packets" << std::endl;

                    int err = srtp_protect(srtpSession2, &packets.front().data , &packets.front().packetLen);
                    if(err == 0) {
                        sendto(sock, (const char*) packets.front().data, packets.front().packetLen, MSG_DONTWAIT,(struct sockaddr *) &addr, len);

                    } else {
                        //std::cout << "error " << err << std::endl;
                    }

                }
                WriteLock wlock(lock);
                packets.erase(packets.begin());
                wlock.unlock();




}

RTPHandler::~RTPHandler() {

    Shutdown();



}





