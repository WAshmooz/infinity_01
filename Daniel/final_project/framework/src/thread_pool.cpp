/* -----------------------------------------------------------------------------
Description: Thread Pool source file
Author: Daniel
Version:
    v0.1 - first implementation 
----------------------------------------------------------------------------- */
#include <iostream> //std::cout

#ifdef DEBUG //use -DDEBUG flag in compilation
#include <map> //std::map (To mark the threads with indexes)
#endif

#include "thread_pool.hpp"


#ifdef DEBUG 
std::map<std::thread::id, int> g_threads; 
std::mutex my_tmp_mutex;
#endif

namespace hrd31
{

void ThreadPool::ExecutionLogic() 
{
    static int cnt = 0;
    try {
        while (true) {

        #ifdef DEBUG
            std::unique_lock<std::mutex> lock2(my_tmp_mutex);
            if (g_threads.find(std::this_thread::get_id()) == g_threads.end()){
                g_threads[std::this_thread::get_id()] = ++cnt;
            } 
            lock2.unlock();
            std::cout <<CYAN<< " T_ID: " << g_threads[std::this_thread::get_id()] 
                                                            <<RESET<< std::endl;
        #endif

            auto task = m_tasks.Pop();
            task->Execute(); 
        }
    } catch (const std::runtime_error& e) {
        std::cout <<RED<< "Caught exception [ExecutionLogic()]: " <<RESET<< e.what() << std::endl;
    }
}


ThreadPool::ThreadPool(size_t size_) 
    : m_tasks([](task_ptr p1_, task_ptr p2_) {return *p1_ < *p2_;}), 
      m_pool_size(size_)
{
    for (size_t i = 0; i < m_pool_size; ++i)
    {
        m_pool.emplace_back(std::bind(&ThreadPool::ExecutionLogic, this));
    }
}

ThreadPool::~ThreadPool()
{
    Stop(); //greacfully
}

void ThreadPool::AddTask(std::shared_ptr<ITask> task_)
{
    m_tasks.Push(task_);
}

void ThreadPool::SetSize(size_t new_size_)
{
    //case 1: reduse
    if (new_size_ < m_pool_size) 
    {
        size_t num_to_pause = m_pool_size - new_size_;
        for (size_t i = 0; i < num_to_pause; ++i) 
        {
            m_tasks.Push(std::make_shared<PauseTask>(*this));
        }
    } 
    //case 2: resume without create
    else if (new_size_ <= m_pool.size()) 
    {
        m_cv.notify_all(); 
        size_t num_to_resume = new_size_ - m_pool_size;
        size_t num_treads_sleeping  = m_pool.size() - m_pool_size;
        for (size_t i = 0; i < (num_treads_sleeping-num_to_resume); ++i) 
        {
            m_tasks.Push(std::make_shared<PauseTask>(*this));
        }
    }
    //case 3: notify_all and create new threads as needed
    else
    {
        m_cv.notify_all();

        size_t num_to_create = new_size_ - m_pool.size();
        for (size_t i = 0; i < num_to_create; ++i) 
        {
            m_pool.emplace_back(std::bind(&ThreadPool::ExecutionLogic, this));
        }
    }

    m_pool_size = new_size_; 
}

void ThreadPool::Pause() 
{
    for (size_t i = 0; i < m_pool_size; ++i) 
    {
        m_tasks.Push(std::make_shared<PauseTask>(*this));
    }
}

void ThreadPool::Resume() 
{
    for (size_t i = 0; i < m_pool_size; ++i) 
    {
        m_cv.notify_one();
    }
}

void ThreadPool::Stop() 
{
    m_cv.notify_all();

    for (size_t i = 0; i < m_pool.size(); ++i) 
    {
         m_tasks.Push(std::make_shared<StopTask>(*this));
    }


    for (auto &thread : m_pool)
    {
        if (thread.joinable())
            try {
                thread.join();
            } 
            catch (const std::exception& e) {
                std::cerr << RED<< "Exception caught [Stop()]: "
                                              << RESET << e.what() << std::endl;
                #ifdef DEBUG 
                std::cout <<CYAN<< " T_ID: " << g_threads[std::this_thread::get_id()] 
                                                            <<RESET<< std::endl;
                #endif
            }
    }
    m_pool.clear();

    #ifdef DEBUG
    std::cout << "Thread Pool stopped gracefully" << std::endl;
    #endif
}

void ThreadPool::Stop(std::chrono::seconds timeout_)
{
    std::this_thread::sleep_for(timeout_);
    m_pool.clear();
    #ifdef DEBUG
    std::cout << "Thread Pool forced to stopp with timeout " << std::endl;
    #endif
}

size_t ThreadPool::DefaultThreadsNum()
{
    auto threads_supported = std::thread::hardware_concurrency();

    //deal case of unknown or not computable threads supported by the hardware 
    return threads_supported > 0 ? threads_supported : 1;
}


}// end hrd31

