#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <iostream>
#include <functional>
#include <memory>

class EventLoop;

class TcpConnection 
: public std::enable_shared_from_this<TcpConnection>
{
    using TcpConnectionCallback =
        std::function<void(const std::shared_ptr<TcpConnection>&)>;

private: 
    EventLoop* _pEvtLoop;
    SocketIO _sockIO;

    Socket _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;

    TcpConnectionCallback _onNewConnection, _onNewMessage, _onNewClose;

private:
    /// @brief 获取服务端地址信息
    /// @return 地址信息存储类
    InetAddress getLocalAddr();

    /// @brief 获取客户端地址信息
    /// @return 地址信息存储类
    InetAddress getPeerAddr();

public: 
    explicit TcpConnection(int fd, EventLoop* evtLp);
    ~TcpConnection();

    /// @brief 发送消息
    /// @param msg 
    void send(const std::string& msg);

    /// @brief 接收消息
    /// @return 接收到的消息
    std::string receive();

    /// @brief 判断TcpConnection是否关闭
    /// @return true:关闭
    bool isClosed();

    /// @brief 
    /// @param msg 
    void sendInLoop(const std::string& msg);

    // 回调函数注册
    void setNewConnectionCallback(const TcpConnectionCallback& cb);
    void setMessageCallback(const TcpConnectionCallback& cb);
    void setCloseCallback(const TcpConnectionCallback& cb);

    // 回调函数实现
    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();


    std::string toString();

};

#endif //_TCPCONNECTION_H