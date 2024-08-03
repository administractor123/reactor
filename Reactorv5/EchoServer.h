#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "TcpConnection.h"
#include "ThreadPool.h"
#include "TcpServer.h"

#include <iostream>

class MyTask
{
private:
    std::string _msg;
    std::shared_ptr<TcpConnection> _con;

public:
    MyTask(const std::string& msg, const std::shared_ptr<TcpConnection>& con);
    void process();

};

class EchoServer
{
private:
    ThreadPool _pool;
    TcpServer _server;

public:
    EchoServer(size_t pNums, size_t qSize, const std::string& ip, unsigned short port);
    ~EchoServer();

    void start();
    void stop();

    void onNewConnection(const std::shared_ptr<TcpConnection>& con);
    void onMessage(const std::shared_ptr<TcpConnection>& con);
    void onColse(const std::shared_ptr<TcpConnection>& con);

};

#endif