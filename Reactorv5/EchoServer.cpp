#include "EchoServer.h"

MyTask::MyTask(const std::string& msg, const std::shared_ptr<TcpConnection>& con)
:_msg(msg), _con(con)
{}

void MyTask::process()
{
    _con->sendInLoop(_msg);
}

EchoServer::EchoServer(size_t tNums, size_t qSize, const std::string& ip, unsigned short port)
:_pool(tNums, qSize), _server(ip, port)
{}

EchoServer::~EchoServer()
{}

void EchoServer::start()
{
    _pool.start();
    using namespace std::placeholders;

    _server.setAllCallback(std::bind(&EchoServer::onNewConnection, this, _1)
    , std::bind(&EchoServer::onMessage, this, _1)
    , std::bind(&EchoServer::onColse, this, _1));

    _server.start();
}

void EchoServer::stop()
{
    _pool.stop();
    _server.stop();
}

void EchoServer::onNewConnection(const std::shared_ptr<TcpConnection>& con)
{
    std::cout<< con->toString() << " has connected" <<std::endl;
}

void EchoServer::onMessage(const std::shared_ptr<TcpConnection>& con)
{
    std::string msg = con->receive();
    std::cout<< ">>recv msg from client: " << msg <<std::endl;

    MyTask task(msg, con);

    _pool.addTask(std::bind(&MyTask::process, task));
}

void EchoServer::onColse(const std::shared_ptr<TcpConnection>& con)
{
    std::cout<< con->toString() << " has closed" <<std::endl;
}

