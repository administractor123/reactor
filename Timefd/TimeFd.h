#ifndef TIMEFD_H
#define TIMEFD_H

#include <functional>

class TimeFd
{
    using TimeFdCallback = std::function<void()>;
private:
    int _timeFd;

    /// @brief 超时后执行操作
    TimeFdCallback _cb;

    bool _isStart;

    /// @brief 定时器起始时间
    int _initSec;

    /// @brief 定时器超时时间
    int _peridocSec;

public:
    TimeFd(TimeFdCallback&& cb, int initSec, int peridocSec);
    ~TimeFd();

    void start();
    void stop();

    int createTimeFd();

    void handleRead();

    void setTimeFd(int initSec, int peridocSec);

};

#endif