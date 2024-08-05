#include "TimeFd.h"
#include <unistd.h>
#include <iostream>
#include <functional>
#include <thread>

using std::cout;
using std::endl;
using std::bind;
using std::thread;

class MyTask
{
public:
    void process()
    {
        cout << "MyTask is running!!!" << endl;
    }
};

int main(int argc, char *argv[])
{
    MyTask task;
    TimeFd tfd(bind(&MyTask::process, &task), 1, 4);

    thread th(bind(&TimeFd::start, &tfd));//th是A线程,主线程是B线程

    sleep(30);

    tfd.stop();
    th.join();
    return 0;
}
