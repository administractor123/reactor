#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Acceptor.h"
#include "TcpConnection.h"
#include <vector>
#include <map>
#include <memory>
#include <sys/epoll.h>
#include <functional>


class EventLoop
{
    using TcpConnectionCallback = 
        std::function<void(const std::shared_ptr<TcpConnection>&)>;

private:
    int _epfd;
    std::vector<struct epoll_event> _evtList;
    bool _isLooping;
    Acceptor& _acceptor;
    std::map<int, std::shared_ptr<TcpConnection>> _conns;

    TcpConnectionCallback _onNewConnection, _onMessage, _onClose;

private:
    /// @brief epoll循环框架：处理连接事件
    void waitEpollFd();

    /// @brief 建立新连接请求
    void handleNewConnection();

    /// @brief 处理旧连接请求
    /// @param fd 
    void handleMessage(int fd);

    int createEpollFd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);

public:
    EventLoop(Acceptor& acc);
    ~EventLoop();
    void loop();
    void unloop();

    void setNewConnectionCallback(TcpConnectionCallback&& cb);
    void setMessageCallback(TcpConnectionCallback&& cb);
    void setCloseCallback(TcpConnectionCallback&& cb);

};

#endif