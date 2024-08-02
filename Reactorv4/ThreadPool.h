#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "TaskQueue.h"
#include <thread>
#include <vector>

class ThreadPool
{
    using Task = std::function<void()>;

private:
    size_t _threadNums;
    std::vector<std::thread> _threadsArr;
    size_t _queSize;
    TaskQueue _taskQue;
    bool _isExit;

public:
    ThreadPool(size_t tNums, size_t qSize);

    /// @brief 线程池起动，创建线程
    void start();

    /// @brief 关闭线程池，销毁线程
    void stop();

    /// @brief 添加线程任务
    /// @param val function<void()>
    void addTask(Task&& val);

    /// @brief 获取线程任务
    /// @return function<void()>
    Task getTask();

    /// @brief 执行线程任务
    void doTask();

};

#endif