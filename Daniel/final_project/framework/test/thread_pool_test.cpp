#include <iostream>
#include <thread>

#include "i_task.hpp"
#include "thread_pool.hpp"

class DummyTask1 : public hrd31::ITask 
{
public:
    DummyTask1(Priority priority = MEDIUM) : ITask(priority) {}

private:
    virtual void Execute() override 
    {
        std::cout << GREEN << "Executing DummyTask1"<< RESET<< std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
    }
};

int test0()
{
    hrd31::ThreadPool thread_pool(1); 

    // Add tasks with different priorities to the thread pool
    for (int i = 0; i < 3; ++i) 
    {
        hrd31::ITask::Priority priority;
        auto task = std::make_shared<DummyTask1>(priority);
        thread_pool.AddTask(task);
    }

    // Pause and resume the thread pool to observe behavior
    thread_pool.Pause();
    std::cout << "\t\t\t\t Thread Pool paused..." << std::endl;

    // Wait for a while to observe the paused state
    //std::this_thread::sleep_for(std::chrono::seconds(2));

    thread_pool.Resume();
    std::cout << "\t\t\t\t Thread Pool resumed..." << std::endl;

    // Stop the thread pool and catch the exception
    try {
        thread_pool.Stop(std::chrono::seconds(2)); // Stop with a timeout of 2 seconds
    } catch (const std::runtime_error& e) {
        std::cout << "\t\t 2(main)    Caught exception: " << e.what() << std::endl;
    }
    return 0;
}


int test1()
{
    hrd31::ThreadPool thread_pool(2); 

    // Add tasks with different priorities to the thread pool
    for (int i = 0; i < 5; ++i) 
    {
        hrd31::ITask::Priority priority;
        auto task = std::make_shared<DummyTask1>(priority);
        thread_pool.AddTask(task);
    }
    // Stop the thread pool and catch the exception
    try {
        thread_pool.Stop(std::chrono::seconds(2)); // Stop with a timeout of 2 seconds
    } catch (const std::runtime_error& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    std::cout << "Thread Pool stopped..." << std::endl;

    return 0;
}

/******************************************************************************/

class DummyTask2 : public hrd31::ITask 
{
public:
    DummyTask2(Priority priority = MEDIUM) : ITask(priority) {}

private:
    virtual void Execute() override 
    {
        // Execution logic for the task
        std::string priority_str;
        switch (GetPriority()) 
        {
            case LOW:
                priority_str = "LOW";
                break;
            case MEDIUM:
                priority_str = "MEDIUM";
                break;
            case HIGH:
                priority_str = "HIGH";
                break;
            case ADMIN:
                priority_str = "ADMIN";
                break;
        }
        std::cout << GREEN<< "Executing DummyTask2 with Priority: " << priority_str<< RESET << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
    }
};


int test2() // 2 threads and 20 task - to see if the pqueue sort works fine 
{
    // Create a ThreadPool with a specified size
    hrd31::ThreadPool thread_pool(10); 

    // Add tasks with different priorities to the thread pool
    for (int i = 0; i < 10; ++i) 
    {
        hrd31::ITask::Priority priority;
        switch (i % 4) { // Vary the priority
            case 0:
                priority = hrd31::ITask::Priority::LOW;
                break;
            case 1:
                priority = hrd31::ITask::Priority::MEDIUM;
                break;
            case 2:
                priority = hrd31::ITask::Priority::HIGH;
                break;
            case 3:
                priority = hrd31::ITask::Priority::ADMIN;
                break;
        }
        auto task = std::make_shared<DummyTask2>(priority);
        thread_pool.AddTask(task);
    }

    // Pause and resume the thread pool to observe behavior
    thread_pool.Pause();
    std::cout << "Thread Pool paused..." << std::endl;

    // Wait for a while to observe the paused state
    std::this_thread::sleep_for(std::chrono::seconds(2));

    thread_pool.Resume();
    std::cout << "Thread Pool resumed..." << std::endl;

    // Change the size of the thread pool (pause on 7 threads)
    thread_pool.SetSize(3);
    std::cout << "\t\t\t\t Thread Pool size changed to 3" << std::endl;

    // Change the size of the thread pool (active 5 threads)
    thread_pool.SetSize(7);
    std::cout << "\t\t\t\t Thread Pool size changed to 5" << std::endl;

    // Change the size of the thread pool (active 5 threads + create 10 more)
    thread_pool.SetSize(15);
    std::cout << "\t\t\t\t Thread Pool size changed to 20" << std::endl;

    // Wait for a while to observe behavior
    std::this_thread::sleep_for(std::chrono::seconds(5));


    // Stop the thread pool and catch the exception
    try {
        thread_pool.Stop(); // Stop gracefully
    } catch (const std::runtime_error& e) 
    {
        std::cout << "/n/n 2(main)    Caught exception: " << e.what() << std::endl;
         std::cout <<CYAN<< " T_ID: " << std::this_thread::get_id() <<RESET<< std::endl;

    }

/*
    // Force to Stop the thread pool and catch the exception
    try {
        thread_pool.Stop(std::chrono::seconds(3)); // Stop gracefully
    } catch (const std::runtime_error& e) 
    {
        std::cout << "/n/n 2(main)    Caught exception: " << e.what() << std::endl;
    }
*/
    return 0;
}

/******************************************************************************/

int main()
{
    return test2();  
}











