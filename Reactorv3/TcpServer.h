#ifndef TCPSEVER_H
#define TCPSEVER_H

#include "Acceptor.h"
#include "EventLoop.h"

class TcpSever
{

    using TcpConnectionCallback
        = std::function<void(const std::shared_ptr<TcpConnection>&)>;

private:
    Acceptor _acceptor;
    EventLoop _evtLoop;

public:
    TcpSever(const std::string& ip, unsigned short port);

    void start();
    void stop();

    void setAllCallback(TcpConnectionCallback&& cb1
                       , TcpConnectionCallback&& cb2
                       , TcpConnectionCallback&& cb3);

};

#endif