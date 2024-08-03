#include "TaskQueue.h"

using ElemType = std::function<void()>;

TaskQueue::TaskQueue(size_t qSize)
:_queSize(qSize), _que(), _mutex()
, _notEmpty(), _notFull(), _flags(true)
{}

void TaskQueue::push(ElemType &&task)
{
    std::unique_lock<std::mutex> _lock(_mutex);

    while (isFull())
    {
        _notFull.wait(_lock);
    }
    
    _que.push(std::move(task));
    _notEmpty.notify_one();
}

ElemType TaskQueue::pop()
{
    std::unique_lock<std::mutex> _lock(_mutex);

    while (isEmpty() && _flags)
    {
        _notEmpty.wait(_lock);
    }
    
    if(_flags)
    {
        auto res = _que.front();
        _que.pop();
        _notFull.notify_one();
        return res;
    }
    else
    {
        return nullptr;
    }

}

void TaskQueue::wakeup()
{
    _flags = false;
    _notEmpty.notify_all();
}


bool TaskQueue::isEmpty()
{
    return 0 == _que.size();
}

bool TaskQueue::isFull()
{
    return _queSize == _que.size();
}
