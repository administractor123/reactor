#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "Socket.h"
#include "InetAddress.h"

/// @brief 初始化TCP协议套接字，使服务端进入listen状态
class Acceptor 
{ 
private: 
    Socket _sock;
    InetAddress _add;
    
private:
    /// @brief 设置地址复用
    void setReusedAddr();

    /// @brief 设置端口复用
    void setReusePort();

    /// @brief 套接字绑定地址
    void bind();

    /// @brief 进入监听状态
    void listen();

public: 
    Acceptor(const std::string& ip, unsigned short port);
    ~Acceptor();

    /// @brief 准备工作：复用设置、地址绑定、监听
    void ready();

    /// @brief 根据_sock套接字提取第一个连接请求
    /// @return 客户端套接字
    int accept();

};

#endif //_ACCEPTOR_H