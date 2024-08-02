#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <iostream>

class TcpConnection 
{
private: 
    SocketIO _sockIO;
    Socket _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;

private:
    /// @brief 获取服务端地址信息
    /// @return 地址信息存储类
    InetAddress getLocalAddr();

    /// @brief 获取客户端地址信息
    /// @return 地址信息存储类
    InetAddress getPeerAddr();

public: 
    explicit TcpConnection(int fd);
    ~TcpConnection();

    /// @brief 发送消息
    /// @param msg 
    void send(const std::string& msg);

    /// @brief 接收消息
    /// @return 接收到的消息
    std::string receive();

    std::string toString();

};

#endif //_TCPCONNECTION_H