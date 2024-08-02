#include "EventLoop.h"
#include <string.h>

EventLoop::EventLoop(Acceptor& acc)
:_epfd(createEpollFd()), _evtList(1024)
, _isLooping(false), _acceptor(acc)
{
    int listen = acc.getFd();
    addEpollReadFd(listen);
}

EventLoop::~EventLoop()
{
    ::close(_epfd);
}

void EventLoop::loop()
{
    _isLooping = true;

    while (_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}

void EventLoop::waitEpollFd()
{
    int nready = 0;
    do
    {
        nready = ::epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 3000);
    } while (-1 == nready && errno == EINTR);
    
    if(-1 == nready)
    {
        std::cout<< "-1 == nready in EventLoop::waitEpollFd()"
        <<std::endl;
        return;
    }
    else if(0 == nready)
    {
        std::cout<< "Epoll wait time out" <<std::endl;
    }
    else
    {
        if(nready == (int)_evtList.size())
        {
            _evtList.resize(2 * nready);
        }

        for (int idx = 0; idx < nready; idx++)
        {
            int fd = _evtList[idx].data.fd;
            int listen = _acceptor.getFd();

            if(fd == listen)
            {
                handleNewConnection();
            }
            else
            {
                handleMessage(fd);
            }
        }
    }
}

void EventLoop::handleNewConnection()
{
    int confd = _acceptor.accept();
    if(confd < 0)
    {
        perror("_accept.accept() in EventLoop::handleNewConnection()");
        return;
    }

    addEpollReadFd(confd);

    std::shared_ptr<TcpConnection> tcpConPtr 
    (new TcpConnection(confd));

    tcpConPtr->setNewConnectionCallback(_onNewConnection);
    tcpConPtr->setMessageCallback(_onMessage);
    tcpConPtr->setCloseCallback(_onClose);

    _conns[confd] = tcpConPtr;

    tcpConPtr->handleNewConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
    auto it = _conns.find(fd);
    if(it != _conns.end())
    {
        bool flag = it->second->isClosed();
        if(flag)
        {
            it->second->handleCloseCallback();
            delEpollReadFd(fd);
            _conns.erase(fd);
        }
        else
        {
            it->second->handleMessageCallback();
        }
    }
    else
    {
        std::cout<< "连接不存在" <<std::endl;
        return;
    }
}

int EventLoop::createEpollFd()
{
    int fd = ::epoll_create(1);
    if(fd < 0)
    {
        perror("epoll_create1() in EventLoop::createEpollFd()");
        return -1;
    }
    return fd;
}

void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if(ret < 0)
    {
        perror("epoll_ctl in EventLoop::addEpollReadFd(int fd)");
        return;
    }
}

void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if(ret < 0)
    {
        perror("epoll_ctl in EventLoop::delEpollReadFd(int fd)");
        return;
    }
}

void EventLoop::setNewConnectionCallback(TcpConnectionCallback&& cb)
{
    _onNewConnection = std::move(cb);
}

void EventLoop::setMessageCallback(TcpConnectionCallback&& cb)
{
    _onMessage = std::move(cb);
}

void EventLoop::setCloseCallback(TcpConnectionCallback&& cb)
{
    _onClose = std::move(cb);
}
