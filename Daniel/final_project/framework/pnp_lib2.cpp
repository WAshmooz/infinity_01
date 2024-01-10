#include <thread>
#include <iostream>
#include <memory>

#include "i_task.hpp"
#include "factory.hpp"
#include "utils.hpp"


//Decleration 
void SharedFunPnp() __attribute__((constructor));


//ITasck Class
class DummyTask : public hrd31::ITask 
{
public:
    DummyTask(Priority priority = MEDIUM) : ITask(priority) {}
	~DummyTask() = default;

private:
    virtual void Execute() override 
    {
        std::cout << GREEN << "Executing DummyTask from shared object Without Factory"<< RESET<< std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
    }

    int m;
};



/******* Will be called automatically when the shared object is loaded   ******/ 

void SharedFunPnp() 
{
    using namespace hrd31;

    DummyTask dummy_task;

    std::cout << GREEN << "Shared object file End!"<< RESET << std::endl;
}


/*
g++ -std=c++11 -shared -fPIC pnp_sample_lib.cpp -o libsample.so

*/


/*
איך לתעד שגיאות:

sudo echo core > /proc/sys/kernel/core_pattern 
sudo bash
ulimit -c unlimited
cgdb a.out core.147151 

rm core*

**************************************************************************
הסבר של מה שכתוב פה למעלה:

Redirects the string "core" to the file located at /proc/sys/kernel/core_pattern

/proc/sys/kernel/core_pattern: This is a special file in the Linux file system 
that determines the filename format for core dumps when a program crashes.

*/

