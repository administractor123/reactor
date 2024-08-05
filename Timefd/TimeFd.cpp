#include "TimeFd.h"
#include <unistd.h>
#include <poll.h>
#include <sys/timerfd.h>
#include <stdio.h>
#include <cerrno>
#include <iostream>

TimeFd::TimeFd(TimeFdCallback&& cb, int initSec, int peridocSec)
:_timeFd(createTimeFd()), _cb(std::move(cb))
, _isStart(false), _initSec(initSec)
, _peridocSec(peridocSec)
{}

TimeFd::~TimeFd()
{
    ::close(_timeFd);
}

void TimeFd::start()
{
    struct pollfd pFd;
    pFd.fd = _timeFd;
    pFd.events = POLLIN;

    setTimeFd(_initSec, _peridocSec);

    _isStart = true;

    while (_isStart)
    {
        int nready = poll(&pFd, 1, 5000);
        if(-1 == nready && errno == EINTR)
        {
            continue;
        }
        else if(-1 == nready)
        {
            std::cerr << "-1 == nready in TimeFd::start()" <<std::endl;
            return;
        }
        else if(0 == nready)
        {
            std::cout<< ">>poll timeout" <<std::endl;
        }
        else
        {
            if(pFd.revents & POLLIN)
            {
                handleRead();
                if(_cb)
                {
                    _cb();
                }
            }
        }
    }
}


void TimeFd::stop()
{
    if(_isStart)
    {
        _isStart = false;
        setTimeFd(0, 0);
    }
}

int TimeFd::createTimeFd()
{
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if(fd < 0)
    {
        perror("timerfd_create in TimeFd::createTimeFd()");
        return;
    }
}

void TimeFd::handleRead()
{
    uint64_t one = 1;
    ssize_t ret = read(_timeFd, &one, sizeof(uint64_t));
    if(ret != sizeof(uint64_t))
    {
        perror("read in TimeFd::handleRead()");
        return;
    }
}

void TimeFd::setTimeFd(int initSec, int peridocSec)
{
    struct itimerspec newValue;
    newValue.it_value.tv_sec = initSec;
    newValue.it_value.tv_nsec = 0;

    newValue.it_interval.tv_sec = peridocSec;
    newValue.it_interval.tv_nsec = 0;

    int ret = timerfd_settime(_timeFd, 0, &newValue, nullptr);
    if(ret < 0)
    {
        perror("timerfd_settime in TimeFd::setTimeFd(int initSec, int peridocSec)");
        return;
    }
}
