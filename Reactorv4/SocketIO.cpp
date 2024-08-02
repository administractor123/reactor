#include "SocketIO.h"
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>

SocketIO::SocketIO(int fd) 
:_fd(fd)
{}

SocketIO::~SocketIO() 
{
    ::close(_fd);
}

int SocketIO::readn(char* buf, int len) 
{
    int left = len;
    char* ptr = buf;
    int ret = 0;

    while (left > 0)
    {
        ret = read(_fd, ptr, left);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("read in SocketIO::readn(char* buf, int len)");
            return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            ptr += ret;
            left -= ret;
        }
    }

    return len - left;
}

int SocketIO::readLine(char* buf, int len) 
{
    int left = len - 1;
    char* ptr = buf;
    int ret = 0, total = 0;

    while (left > 0)
    {
        ret = recv(_fd, ptr, left, MSG_PEEK);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("recv in SocketIO::readLine(char* buf, int len)");
            return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            for (int idx = 0; idx < ret; ++idx)
            {
                if(ptr[idx] == '\n')
                {
                    int sz = idx + 1;
                    readn(ptr, sz);
                    ptr += sz;
                    *ptr = '\0';

                    return total + sz;
                }
            }

            readn(ptr, ret);
            total += ret;
            ptr += ret;
            left -=ret;            
        }
    }
    
    *ptr = '\0';

    return total - left;
}

int SocketIO::writen(const char* buf, int len) 
{
    int left = len;
    const char* ptr = buf;
    int ret = 0;

    while (left > 0)
    {
        ret = write(_fd, ptr, left);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("write in SocketIO::writen(const char* buf, int len)");
            return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            ptr += ret;
            left -= ret;
        }
    }
    
    return len - left;
}

