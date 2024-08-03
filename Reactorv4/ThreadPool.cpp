#include "ThreadPool.h"
#include <iostream>

using Task = std::function<void()>;

ThreadPool::ThreadPool(size_t tNums, size_t qSize)
:_threadNums(tNums), _threadsArr()
, _queSize(qSize), _taskQue(qSize)
, _isExit(false)
{}

void ThreadPool::start()
{
    for (size_t i = 0; i != _threadNums; i++)
    {
        _threadsArr.push_back(std::thread(&ThreadPool::doTask, this));
    }
    
}

void ThreadPool::stop()
{
    while (!_taskQue.isEmpty())
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    _isExit = true;
    _taskQue.wakeup();

    for (auto& it: _threadsArr)
    {
        it.join();
    }
    

}

void ThreadPool::addTask(Task&& val)
{
    if(val)
    {
        _taskQue.push(std::move(val));
    }
}

Task ThreadPool::getTask()
{
    return _taskQue.pop();
}

void ThreadPool::doTask()
{
    if(!_isExit)
    {
        Task taskcb = getTask();
        if(taskcb)
        {
            taskcb();
        }
        else
        {
            std::cout<< "task == nullptr" <<std::endl;
        }
    }
}
