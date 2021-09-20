#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <iostream>
void CONSOLELOG(std::string msg) {
#ifdef WEBRTC_DEBUG
    //std::cout << "(" << __FILE__ << ":" << __LINE__ <<") " << msg << std::endl;
#endif

}

#endif // DEBUG_H_INCLUDED
