#ifndef _SOCKET_H
#define _SOCKET_H

#include "NoCopyable.h"

class Socket 
: public NoCopyable
{
private: 
    int _fd;

public: 
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int getFd();

};

#endif //_SOCKET_H