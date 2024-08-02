#include "InetAddress.h"
#include <cstring>

InetAddress::InetAddress(const std::string& ip, unsigned short port) 
{
    ::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    _addr.sin_port = htons(port);
}

InetAddress::InetAddress(const struct sockaddr_in& add) 
:_addr(add)
{}

InetAddress::~InetAddress() 
{}

std::string InetAddress::ip() 
{
    return std::string(inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() 
{
    return ntohs(_addr.sin_port);
}

struct sockaddr_in* InetAddress::getAddressPtr() 
{
    return &_addr;
}