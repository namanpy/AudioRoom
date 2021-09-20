#ifndef STUN_H_INCLUDED
#define STUN_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/hmac.h>
#include <algorithm>
#include <boost/crc.hpp>

#ifdef _DEBUG
#define DBG_NEW new
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif



class STUN {

public:
    static bool IsStun(unsigned char* packet) {

        if(packet[0] == 0x00 && packet[1] == 0x01) {
            return true;
        } else if(packet[0] == 0x00 && packet[1] == 0x01) {
            return true;
        }
        return false;
    }

    static bool IsBindingRequest(unsigned char* packet) {
        if(packet[0] == 0x00 && packet[1] == 0x01) {
            return true;
        }
        return false;
    }

    static bool IsBindingResponse(unsigned char* packet) {
        if(packet[0] == 0x01 && packet[1] == 0x01) {
            return true;
        }
        return false;
    }

    static int CharToInt(unsigned char* data, int start, int len) {
        int value = 0;

        for(int i = start; i < start + len; i++) {
//             //std::cout << "ree value " << data[i] << std::endl;
            value = (value << (sizeof(data[i]) * 8)) | data[i];
        }
        return value;
    }

    static void XORMapAddress(unsigned char* address) {

        address[0] = address[0] ^ magicCookie_[0];
        address[1] = address[1] ^ magicCookie_[1];
        address[2] = address[2] ^ magicCookie_[2];
        address[3] = address[3] ^ magicCookie_[3];
    }

    static void XORMapPort(unsigned int port, unsigned char* out) {

        out[0] = ((port >> 0) & 0xFF) ^ magicCookie_[0];
        out[1] = ((port >> 8) & 0xFF) ^ magicCookie_[1];
    }
    void Parse(unsigned char* packet, int len) {

        type[0] = packet[0];
        type[1] = packet[1];

        messageLength = CharToInt(packet, 2, 2);

        ////std::cout << "Message Length is " << messageLength << std::endl;


        bool equal = true;
        for(int i = 0; i < 4; i++) {
             if(magicCookie[0 + i] != packet[4 + i]) {
                ////std::cout << "unequal at " << (unsigned int) packet[4 + i]  << std::endl;
                 equal = false;
                 break;
            } else {

            }

        }
        if(equal) {
            ////std::cout << "cookies are equal" << std::endl;
        }

        memcpy(transactionId, packet + 8, 12);

//data = DBG_NEW unsigned char[messageLength + 1];

        memcpy( data, packet + 20, messageLength);

    }


    void Init(int stuntype, unsigned char transactionid[]) {
        if(stuntype == StunType::BINDING_RESPONSE) {
            type[0] = 0x01;
            type[1] = 0x01;
        } else if(stuntype == StunType::BINDING_REQUEST)  {
            type[0] = 0x00;
            type[1] = 0x01;
        }

        memcpy(transactionId, transactionid, sizeof transactionId);

        data_[0] = type[0];
        data_[1] = type[1];
        data_[2] = 0x00;
        data_[3] = 0x0c;
        memcpy(data_ + 4, magicCookie, 4);
        memcpy(data_ + 8, transactionId, sizeof transactionId);
        len += 20;

    }

    void ToBytes(unsigned int value, int len_, unsigned char* out) {

        for(int i = 0; i < len_; i++) {
            out[i] = (value >>  8 * (i)) & 0xFF;
        }

    }

    void ToBytesRev(unsigned int value, int len_, unsigned char* out) {

        for(int i = 0; i < len_; i++) {
            out[i] = (value >>  8 * (len_ - i - 1)) & 0xFF;
        }

    }


    void AddXORMappedAddress(unsigned int address, unsigned int port) {

        unsigned char addressBytes[4];
        unsigned char portBytes[2];

        data_[ len + 0] = 0x00;
        data_[ len + 1] = 0x20;
        data_[ len + 2] = 0x00;
        data_[ len + 3] = 0x08;
        data_[ len + 4] = 0x00;
        data_[ len + 5] = 0x01;

        XORMapPort(port, portBytes);
        memcpy( data_  + len + 6, portBytes, 2);

        ////std::cout << "address as int is " << address << std::endl;

        ToBytes(address, 4, addressBytes);

        XORMapAddress(addressBytes);
        //std::reverse(addressBytes, addressBytes + 3);
        memcpy( data_ + len + 8, addressBytes, 4);

        len += 12;
        dataLen += 12;

    }

    void AddMessageIntegrity(unsigned char* key, int keysize) {

        unsigned char digest[20];
        unsigned int len_ = 0;

        data_[len + 0] = 0x00;
        data_[len + 1] = 0x08;
        data_[len + 2] = 0x00;
        data_[len + 3] = 0x14;

        len += 24;
        dataLen += 24;




        unsigned char packetLen[2];
        ToBytes(dataLen, 2, packetLen);

        data_[2] = packetLen[1];
        data_[3] = packetLen[0];

        HMAC(EVP_sha1(), key, keysize, data_, len - 24, digest, &len_);

        ////std::cout << "digest is : " << std::endl;
        for(int i = 0; i < len_; i++) {
            //std::cout << (int) digest[i];
        }

        //std::cout << std::endl;
        for(int i = 0; i < 20; i++) {
            data_[len - 20 + i] = digest[i];
        }
    }

    unsigned int GetCrc32(unsigned char* crcdata, unsigned int len_) {
        boost::crc_32_type result;
        result.process_bytes(crcdata, len_);
        return result.checksum();
    }

    void AddFingerprint() {

        len += 8;
        dataLen += 8;


        unsigned char packetLen[2];
        ToBytes(dataLen, 2, packetLen);

        data_[2] = packetLen[1];
        data_[3] = packetLen[0];


        unsigned int crc32 = GetCrc32(data_, len - 8)  ^ 0x5354554e;

        unsigned char crc32bytes[4];

        ToBytes(crc32, 4, crc32bytes);

        data_[len  - 8 + 0] = 0x80;
        data_[len  - 8 + 1] = 0x28;
        data_[len  - 8 + 2] = 0x00;
        data_[len  - 8 + 3] = 0x04;

        data_[len - 8 + 4] = crc32bytes[3];
        data_[len - 8 + 5] = crc32bytes[2];
        data_[len - 8 + 6] = crc32bytes[1];
        data_[len - 8 + 7] = crc32bytes[0];


    }

    ~STUN() {
    
        //delete data;
    
    }


    enum StunType { BINDING_REQUEST, BINDING_RESPONSE };

    char type[2];
    int messageLength;
    unsigned char magicCookie[4] = { 0x21, 0x12, 0xA4, 0x42 };
    constexpr static const unsigned char magicCookie_[4] = { 0x21, 0x12, 0xA4, 0x42 };
    unsigned char transactionId[12];
    unsigned char data[1500];
    unsigned char data_[1500]; //Full Packet Data INCLUDING Header
    unsigned int len = 0;
    unsigned int dataLen = 0;
    unsigned int preMessageIntegrityLen = 0;

};

#endif // STUN_H_INCLUDED
