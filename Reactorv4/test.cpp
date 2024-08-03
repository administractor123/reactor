#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpServer.h"
#include "ThreadPool.h"
#include "TcpConnection.h"
#include <iostream>
#include <unistd.h>

ThreadPool *gPool = nullptr;

class MyTask
{
private:
    std::string _msg;
    std::shared_ptr<TcpConnection> _con;

public:
    MyTask(const std::string& msg, const std::shared_ptr<TcpConnection>& con)
    :_msg(msg), _con(con)
    {}

    void process()
    {
        _con->sendInLoop(_msg);
    }
};

// void test01()
// {
//     Acceptor acc("127.0.0.1", 8888);
//     acc.ready();

//     TcpConnection con(acc.accept());

//     std::cout<< con.toString() <<std::endl;

//     while (1)
//     {
//         std::cout<< ">> recv msg from client: " << con.receive()
//         << std::endl;
//         con.send("hello from sever!!!");
//     }
// }

// void test02()
// {
//     Acceptor acc("127.0.0.1", 8888);
//     acc.ready();

//     EventLoop el(acc);
//     el.loop();
// }

void onNewConnection(const std::shared_ptr<TcpConnection>& con)
{
    std::cout<< con->toString() << " has connect" <<std::endl;
}

void onMessage(const std::shared_ptr<TcpConnection>& con)
{
    std::string msg = con->receive();
    std::cout<< ">>recv msg from client: " << msg <<std::endl;

    MyTask task(msg, con);
    gPool->addTask(std::bind(&MyTask::process, task));
}

void onClose(const std::shared_ptr<TcpConnection>& con)
{
    std::cout<< con->toString() << " has closed" <<std::endl;
}

// void test03()
// {
//     Acceptor acp("127.0.0.1", 8888);
//     acp.ready();

//     EventLoop evloop(acp);
//     evloop.setNewConnectionCallback(std::move(onNewConnection));
//     evloop.setMessageCallback(std::move(onMessage));
//     evloop.setCloseCallback(std::move(onClose));
//     evloop.loop();
// }


void test04()
{
    ThreadPool pool(4, 10);
    pool.start();
    gPool = &pool;

    TcpServer tcp("127.0.0.1", 8000);
    tcp.setAllCallback(std::move(onNewConnection)
                       , std::move(onMessage)
                       , std::move(onClose));

    tcp.start();
}

int main(int argc, char* argv[])
{
    test04();
    return 0;
}