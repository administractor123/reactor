#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include <iostream>
#include <arpa/inet.h>

class InetAddress 
{
private: 
    struct sockaddr_in _addr;

public: 
    InetAddress(const std::string& ip, unsigned short port);
    InetAddress(const struct sockaddr_in& add);
    ~InetAddress();
    std::string ip();
    unsigned short port();
    struct sockaddr_in* getAddressPtr();

};

#endif //_INETADDRESS_H