#include "Acceptor.h"

Acceptor::Acceptor(const std::string& ip, unsigned short port) 
:_sock(), _add(ip, port)
{}

Acceptor::~Acceptor() 
{}

void Acceptor::ready() 
{
    setReusedAddr();
    setReusePort();
    bind();
    listen();

    std::cout<< "Start listening" <<std::endl;
}

void Acceptor::setReusedAddr() 
{
    int opt = 1;
    if(-1 == setsockopt(_sock.getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt in Acceptor::setReusedAddr()");
        return;
    }
}

void Acceptor::setReusePort() 
{
    int opt = 1;
    if(-1 == setsockopt(_sock.getFd(), SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt in Acceptor::setReusePort()");
        return;
    }
}

void Acceptor::bind() 
{
    int res = ::bind(_sock.getFd()
    , (struct sockaddr*)_add.getAddressPtr()
    , sizeof(struct sockaddr));

    if (-1 == res)
    {
        perror("bind in Acceptor::bind()");
        return;
    }
}

void Acceptor::listen() 
{
    if(-1 == ::listen(_sock.getFd(), 128))
    {
        perror("listen in Acceptor::listen()");
        return;
    }
}

int Acceptor::accept() 
{
    int conFd = ::accept(_sock.getFd(), nullptr, nullptr);
    if(-1 == conFd)
    {
        perror("accept in Acceptor::accept()");
        return -1;
    }
    return conFd;
}


int Acceptor::getFd()
{
    return _sock.getFd();
}