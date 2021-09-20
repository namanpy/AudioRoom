
#include <iostream>

//#include <execinfo.h>
#include <signal.h>
#include <cstdlib>
//#include <unistd.h>


#include <stdlib.h>


#ifdef _DEBUG
#define DBG_NEW new
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include <boost/beast/core.hpp>


#include "SignalServer.cpp"
#include "EventEmitter.cpp"
#include <nlohmann/json.hpp>
#include "CallServer/CallServer.h"
#include "Certificate.h"
#define _ITERATOR_DEBUG_LEVEL 0



using json = nlohmann::json;





int main(int argc, char* argv[])
{

#if defined(DEBUG) | defined(_DEBUG)
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF); // added in _CRTDBG_CHECK_ALWAYS_DF
    _CrtSetBreakAlloc(0);
#endif

   //signal(SIGSEGV, error_handler);

    CallServer cs;
	EventEmitter e(&cs);

    Certificate cert;
    cert.Init();
	
//	e.on("connect", &CallServer::onConnect);
	e.on("offer", &CallServer::onOffer);
    e.on("joinRoom", &CallServer::onJoinRoom);
    e.on("iceCandidate", &CallServer::onICECandidate);
    e.on("requestUserList", &CallServer::onRequestUserList);
    e.on("~answer", &CallServer::onAnswer);
	e.on("~requestUserMedia", &CallServer::onRequestUserMedia);
    e.on("~iceCandidate", &CallServer::onRecvMediaICECandidate);
    e.on("disconnect", &CallServer::onDisconnect);
    e.on("muteaudio", &CallServer::onToggleAudio);
    e.on("mutevideo", &CallServer::onToggleVideo);
    e.on("auth", &CallServer::doAuth);

    SignalServer server("0.0.0.0", "8080");
	server.setEventEmitter(&e);
	server.run();

	std::cout << "Waiting for another ddeath" << std::endl;

}
