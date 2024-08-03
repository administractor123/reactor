#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>

class TaskQueue
{
    using ElemType = std::function<void()>;

private:
    size_t _queSize;
    std::queue<ElemType> _que;
    std::mutex _mutex;
    std::condition_variable _notEmpty;
    std::condition_variable _notFull;
    bool _flags;

public:
    TaskQueue(size_t qSize);

    void push(ElemType &&task);
    ElemType pop(); 

    void wakeup();

    bool isEmpty();
    bool isFull();

};

#endif