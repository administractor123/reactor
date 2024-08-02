#include "EventLoop.h"
#include <string.h>
#include <sys/eventfd.h>

EventLoop::EventLoop(Acceptor& acc)
:_epfd(createEpollFd()), _evtList(1024)
, _isLooping(false), _acceptor(acc)
, _evtFd(createEventFd()), _mutex()
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
    (new TcpConnection(confd, this));

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

int EventLoop::createEventFd()
{
    int fd = eventfd(10, 0);
    if(fd < 0)
    {
        perror("eventfd in EventLoop::createEventFd()");
        return;
    }

    return fd;
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t ret = read(_evtFd, &one, sizeof(uint64_t));
    if(ret != sizeof(uint64_t))
    {
        perror("read in EventLoop::handleRead()");
        return;
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t ret = write(_evtFd, &one, sizeof(uint64_t));
    if(ret != sizeof(uint64_t))
    {
        perror("write in EventLoop::handleRead()");
        return;
    }
}

void EventLoop::doPengdingFunction()
{
    std::vector<Functors> tmp;
    {
        std::lock_guard<std::mutex> _lock(_mutex);
        tmp.swap(_pendings);
    }

    for (auto& it: tmp)
    {
        it();
    }
}

void EventLoop::runInLoop(Functors&& cb)
{
    {
        std::lock_guard<std::mutex> _lock(_mutex);
        _pendings.push_back(std::move(cb));
    }

    wakeup();
}
