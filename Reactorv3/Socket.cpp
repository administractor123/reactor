#include "Socket.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>

Socket::Socket() 
{
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_fd < 0)
    {
        perror("socket in Socket::Socket()");
        return;
    }
}

Socket::Socket(int fd) 
:_fd(fd)
{}

Socket::~Socket() 
{
    ::close(_fd);
}

int Socket::getFd() 
{
    return _fd;
}