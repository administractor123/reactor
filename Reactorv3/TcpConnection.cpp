#include "TcpConnection.h"
#include <climits>
#include <sstream>

TcpConnection::TcpConnection(int fd) 
:_sockIO(fd), _sock(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())
{}

TcpConnection::~TcpConnection() 
{}

std::string TcpConnection::receive() 
{
    char buff[0xffff] = {0};
    _sockIO.readLine(buff, sizeof(buff));

    return std::string(buff);
}

void TcpConnection::send(const std::string& msg) 
{
    _sockIO.writen(msg.c_str(), msg.size());
}

bool TcpConnection::isClosed() 
{
    char buff[10] = {0};
    int ret = ::recv(_sock.getFd(), buff, sizeof(buff), MSG_PEEK);

    return (0 == ret);
}

std::string TcpConnection::toString() 
{
    std::ostringstream strS;
    strS << _localAddr.ip() << ":"
    << _localAddr.port() << "---->" 
    << _peerAddr.ip() << ":"
    << _peerAddr.port();

    return strS.str();
}

InetAddress TcpConnection::getLocalAddr() 
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getsockname(_sock.getFd(), (struct sockaddr*)&addr, &len);
    if(-1 == ret)
    {
        perror("getsockname in TcpConnection::getLocalAddr()");
    }

    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr() 
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getpeername(_sock.getFd(), (struct sockaddr*)&addr, &len);
    if(-1 == ret)
    {
        perror("getpeername in TcpConnection::getPeerAddr()");
    }

    return InetAddress(addr);
}

void TcpConnection::setNewConnectionCallback(const TcpConnectionCallback& cb)
{
    _onNewConnection = cb;
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback& cb)
{
    _onNewMessage = cb;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback& cb)
{
    _onNewClose = cb;
}

void TcpConnection::handleNewConnectionCallback()
{
    if(_onNewConnection)
    {
        _onNewConnection(shared_from_this());
    }
    else
    {
        std::cout<< "nullptr == _onNewConnection" <<std::endl;
    }
}

void TcpConnection::handleMessageCallback()
{
    if(_onNewMessage)
    {
        _onNewMessage(shared_from_this());
    }
    else
    {
        std::cout<< "nullptr == _onNewMessage" <<std::endl;
    }
}

void TcpConnection::handleCloseCallback()
{
    if(_onNewClose)
    {
        _onNewClose(shared_from_this());
    }
    else
    {
        std::cout<< "nullptr == _onNewClose" <<std::endl;
    }
}
