cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

set(OPUS_INCLUDE_DIRECTORY "/root/Desktop/AudioRoom/ThirdParty/Opus/libopus/include")
#/src folder of WebRTC Native
set(WEBRTC_INCLUDE_DIRECTORY "C:/Users/naman/Desktop/Development/AudioRoom/ThirdParty/WebRTC/src")
#/include folder in Srtp library
set(LIBSRTP_INCLUDE_DIRECTORY "/root/Desktop/AudioRoom/ThirdParty/libsrtp/include/")

set(BOOST_INCLUDE_DIRECTORY "C:/Users/naman/Desktop/Development/AudioRoom/ThirdParty/boost_1_76_0")


set(JSON_INCLUDE_DIRECTORY "/root/Desktop/AudioRoom/ThirdParty/json/include/include")

set(OPENSSL_INCLUDE_DIRECTORY "C:/Users/naman/Desktop/Development/AudioRoom/ThirdParty/openssl-1.1.1k/include")

set(CURL_INCLUDE_DIRECTORY "C:/Users/naman/Desktop/Development/AudioRoom/ThirdParty/curl-7.78.0/builds/libcurl-vc-x64-release-dll-ipv6-sspi-schannel/include")


set(LIBSRTP_LIBRARY "C:/Users/naman/Desktop/Development/AudioRoom/ThirdParty/libsrtp/build/Debug/srtp2.lib")

set(LIBSSL_LIBRARY "C:/Users/naman/Desktop/Development/AudioRoom/ThirdParty/openssl-1.1.1k/libssl.lib")

set(LIBCRYPTO_LIBRARY "C:/Users/naman/Desktop/Development/AudioRoom/ThirdParty/openssl-1.1.1k/libcrypto.lib")

set(BOOST_LIBRARY "C:/Users/naman/Desktop/Development/AudioRoom/ThirdParty/boost_1_76_0/stage/lib")


set(CURL_LIBRARY "C:/Users/naman/Desktop/Development/AudioRoom/ThirdParty/curl-7.78.0/builds/libcurl-vc-x64-release-dll-ipv6-sspi-schannel/lib/libcurl.lib")

set(OPUS_LIBRARY "C:/Users/naman/Desktop/Development/AudioRoom/ThirdParty/Opus/win32/VS2015/x64/Debug/opus.lib")


project(audioroom)

include(${PROJECT_SOURCE_DIR}/ThirdParty/VoiceActivityDetection/CMakeLists.txt)




add_executable(audioroom ${PROJECT_SOURCE_DIR}/Certificate.cpp ${PROJECT_SOURCE_DIR}/WebRTC/RTPPacket.cc  ${PROJECT_SOURCE_DIR}/WebRTC/PeerConnection.cc ${PROJECT_SOURCE_DIR}/WebRTC/Sdp.cc ${PROJECT_SOURCE_DIR}/WebRTC/Ice.cc ${PROJECT_SOURCE_DIR}/WebRTC/RtpHandler.cc ${PROJECT_SOURCE_DIR}/SignalServer.cpp  ${PROJECT_SOURCE_DIR}/main.cpp  ${PROJECT_SOURCE_DIR}/EventEmitter.cpp   ${PROJECT_SOURCE_DIR}/CallServer/CallServer.cpp)





if(MSVC)
    target_link_options(audioroom PUBLIC /DEBUG)
    set(CMAKE_CXX_FLAGS_RELEASE "/MD")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd /DEBUG")
    add_definitions(-D_ITERATOR_DEBUG_LEVEL=2)


endif()


add_definitions(-DWEBRTC_DEBUG)

 

target_link_libraries(audioroom webrtcvad)
#link_libraries(${BOOST_LIBRARY})

#target_link_libraries(audioroom ${CURL_LIBRARY})
#target_link_libraries(audioroom ${LIBSSL_LIBRARY})
#target_link_libraries(audioroom ${LIBCRYPTO_LIBRARY})
#target_link_libraries(audioroom ${LIBSRTP_LIBRARY})
target_link_libraries(audioroom -pthread)
target_link_libraries(audioroom -lssl)
target_link_libraries(audioroom -lcrypto)
target_link_libraries(audioroom -lcurl)
target_link_libraries(audioroom -lboost_thread)
target_link_libraries(audioroom -lboost_random)
target_link_libraries(audioroom -lboost_chrono)
target_link_libraries(audioroom -lopus)
target_link_libraries(audioroom -lsrtp2)
#target_link_libraries(audioroom ${OPUS_LIBRARY})
#
#target_link_libraries(audioroom "${BOOST_LIBRARY}/libboost_thread-vc142-mt-sgd-x64-1_76.lib")
#target_link_libraries(audioroom "${BOOST_LIBRARY}/libboost_date_time-vc142-mt-sgd-x64-1_76.lib")
#target_link_libraries(audioroom "${BOOST_LIBRARY}/libboost_chrono-vc142-mt-sgd-x64-1_76.lib")
#target_link_libraries(audioroom "${BOOST_LIBRARY}/libboost_random-vc142-mt-sgd-x64-1_76.lib")



#include_directories(${BOOST_INCLUDE_DIRECTORY})
include_directories(${LIBSRTP_INCLUDE_DIRECTORY})
include_directories(${JSON_INCLUDE_DIRECTORY})
#include_directories(${WEBRTC_INCLUDE_DIRECTORY})
#include_directories(${OPENSSL_INCLUDE_DIRECTORY})
include_directories(${OPUS_INCLUDE_DIRECTORY})
#include_directories(${CURL_INCLUDE_DIRECTORY})
#include_directories("/home/naman/Desktop/Call Server/dependencies/WebRTC/src/third_party/abseil-cpp/")

install(TARGETS audioroom RUNTIME DESTINATION bin)
