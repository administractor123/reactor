#include "Acceptor.h"
#include "TcpConnection.h"
#include <iostream>
#include <unistd.h>

void test01()
{
    Acceptor acc("127.0.0.1", 8888);
    acc.ready();

    TcpConnection con(acc.accept());

    std::cout<< con.toString() <<std::endl;

    while (1)
    {
        std::cout<< ">> recv msg from client: " << con.receive()
        << std::endl;
        con.send("hello from sever!!!");
    }
}

int main(int argc, char* argv[])
{
    test01();
    return 0;
}