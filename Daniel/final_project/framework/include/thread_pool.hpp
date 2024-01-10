/* -----------------------------------------------------------------------------
Description: 
Author: HRD31
Reviewer: Itzik
Version:
    v0.1 - For mentor approval
    v0.2 - Mentor approved
----------------------------------------------------------------------------- */
#ifndef __HRD31_THREAD_POOL_HPP__
#define __HRD31_THREAD_POOL_HPP__

#include <memory> //std::shared_ptr
#include <vector> //std::vector
#include <thread> //threads 
#include <functional> //std::function (WPQueue)
#include <chrono> //std::runtime_error

#include "waitable_pq.hpp"
#include "i_task.hpp"
#include "utils.hpp" //DEBUG_ONLY(ThreadPool cpp), ANSI color 

namespace hrd31
{

class PauseTask; // Forward declaration

class ThreadPool final
{
public:
    //DefaultThreadsNum - returns the number of concurrent threads the 
    //implementation supports;
    //if the value is not well defined it returns 1.
    explicit ThreadPool(size_t size_ = DefaultThreadsNum()); 
    ~ThreadPool();

    ThreadPool(const ThreadPool& other_) = delete;
    ThreadPool& operator=(const ThreadPool& other_) = delete;

    void AddTask(std::shared_ptr<ITask> task_);
    void SetSize(size_t new_size_); //trow execption
    void Pause();
    void Resume();
    void Stop();
    void Stop(std::chrono::seconds timeout_);

private:
    static size_t DefaultThreadsNum(); // wrapper for std::thread::hardware_concurrency()

    using task_ptr = std::shared_ptr<ITask>;
    using Container = std::vector<task_ptr>;
    using Compare = std::function<bool(task_ptr p1_, task_ptr p2_)>;

    WPQueue<task_ptr, Container, Compare>   m_tasks;
    std::vector<std::thread>                m_pool;
    size_t                                  m_pool_size;
    std::condition_variable                 m_cv;
    mutable std::mutex                      m_mutex;

    class PauseTask;
    class StopTask;
    void ExecutionLogic();
};



/* -----------------------------------------------------------------------------
								inline header
----------------------------------------------------------------------------- */
class ThreadPool::PauseTask : public ITask 
{
public:
    PauseTask(ThreadPool& pool) : m_pool(pool) {}

private:
    virtual void Execute() override 
    {
        std::unique_lock<std::mutex> lock(m_pool.m_mutex);
        m_pool.m_cv.wait(lock);
    }

    ThreadPool& m_pool;
};


class ThreadPool::StopTask : public ITask 
{
public:
    StopTask(ThreadPool& pool) : m_pool(pool) {}

private:
    virtual void Execute() override 
    {
        throw std::runtime_error("ThreadPool stopped");
    }

    ThreadPool& m_pool;
};


}//namespace hrd31

#endif //__HRD31_THREAD_POOL_HPP__
