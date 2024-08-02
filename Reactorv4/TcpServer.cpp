#include "TcpServer.h"

TcpSever::TcpSever(const std::string& ip, unsigned short port)
:_acceptor(ip, port), _evtLoop(_acceptor)
{}

void TcpSever::start()
{
    _acceptor.ready();
    _evtLoop.loop();
}

void TcpSever::stop()
{
    _evtLoop.unloop();
}

void TcpSever::setAllCallback(TcpConnectionCallback&& cb1
                             , TcpConnectionCallback&& cb2
                             , TcpConnectionCallback&& cb3)
{
    _evtLoop.setNewConnectionCallback(std::move(cb1));
    _evtLoop.setMessageCallback(std::move(cb2));
    _evtLoop.setCloseCallback(std::move(cb3));
}