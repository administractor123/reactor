基本TCP服务端框架，加入epoll实现IO多路复用

Socket类：
存放套接字的类

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

EventLoop类：
对epoll的封装, 包含epoll循环框架, 用于建立新连接、处理旧连接申请任务
1. void loop(); 开始循环
2. void unloop(); 结束循环


v2.1
添加三个事件的回调框架：连接建立、连接断开、消息到达；
在三个事件发生前，注册回调函数框架，等待事件发生；
当这三个事件发生时，程序在通过回调函数执行任务。

注册回调函数：
EventLoop类：
    std::function<void(const std::shared_ptr<TcpConnection> &)> _onNewConnection; 
    std::function<void(const std::shared_ptr<TcpConnection> &)> _onMessage;
    std::function<void(const std::shared_ptr<TcpConnection> &)> _onClose;

    // 注册
    void setNewConnectionCallback(TcpConnectionCallback&& cb);
    void setMessageCallback(TcpConnectionCallback&& cb);
    void setCloseCallback(TcpConnectionCallback&& cb);

实现(执行)回调函数：
TcpConnection类：
    std::function<void(const std::shared_ptr<TcpConnection> &)> _onNewConnection; 
    std::function<void(const std::shared_ptr<TcpConnection> &)> _onMessage;
    std::function<void(const std::shared_ptr<TcpConnection> &)> _onClose;

    // 注册
    void setNewConnectionCallback(TcpConnectionCallback& cb);
    void setMessageCallback(TcpConnectionCallback& cb);
    void setCloseCallback(TcpConnectionCallback& cb);

    // 实现
    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

v3
添加TcpSever类，进一步封装服务端操作
TcpSever类：
1. void start(); 封装了acceptor.start()和EventLoop.loop();
2. void stop();
3. setAllCallback();