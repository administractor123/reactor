基本TCP服务端框架，不涉及IO多路复用

Acceptor类：
该类进行TCP的准备工作
1. void ready(); 用于套接字和IP、端口地址的绑定，以及进入监听状态，
2. int accept(); 返回连接请求的套接字

TcpConnection类：
通过该类实现对服务端的操作
1. string receive(); 接收消息
2. void send(); 发送消息

InetAddress类：
struct sockaddr_in 的封装,实现了网络地址和人类可读地址的转换,同时存放了
地址信息：IP、端口号、协议等

SocketIO类：
实现通过套接字接收、写入数据的操作
1. int readLine(char* buf, int len); 从套接字中读取数据
2. int writen(const char* buf, int len); 向套接字中写入数据
