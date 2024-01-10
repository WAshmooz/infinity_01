/*------------------------------------------------------------------------------
Description: Request Engine FrameWork
             This method plays a crucial role in setting up and configuring 
             input sources within the Reactor framework. It establishes the 
             association between fd and their corresponding input source objects 
             in the m_req_srcs_map map and adds these fd to the epoll watch list, 
             allowing the Reactor to monitor these sources for incoming data or 
             events and handle them accordingly within the ReactorThread execution.
Author: HRD31
Reviewer: Itzik
Version:
    v0.1 - For mentor approval
    v0.2 - Mentor approved
    v0.2.1 - class Stop and Pause are changed to PauseTask and StopTask
----------------------------------------------------------------------------- */
#ifndef __HRD31_RE_FW_HPP__
#define __HRD31_RE_FW_HPP__

#include <string>
#include <memory> //std::shared_ptr
#include <unordered_map>
#include <sys/epoll.h>
#include <unistd.h> //close
// #include <stdexcept>

#include "factory.hpp"
#include "thread_pool.hpp"
#include "pnp.hpp"
#include "i_input_src.hpp"
#include "i_re_task.hpp"

namespace hrd31
{
    size_t DefaultThreadsNum()
    {
        auto threads_supported = std::thread::hardware_concurrency();

        //deal case of unknown or not computable threads supported by the hardware 
        return threads_supported > 0 ? threads_supported : 1;
    }

template <typename KEY, typename ARGS> 
class RequestEngine final
{
public:
    //DefaultThreadsNum - returns the number of concurrent threads the 
    //implementation supports;
    //if the value is not well defined it returns 1.

    explicit RequestEngine(size_t thread_num_ = DefaultThreadsNum(),
        const std::string& pnp_path_ = "./pnp"); //Ctor RequestEngine with default thread number 
                                                 //and a path for PlugNPlay (pnp) settings.
    ~RequestEngine();
    RequestEngine(const RequestEngine&) = delete;
    RequestEngine& operator=(const RequestEngine&) = delete;

    void ConfigInputSrc(std::shared_ptr<IInputSrc<KEY, ARGS>> input_src_);  //Configures input sources by adding 
                                                            //their fd to the epoll watch list.

    using CreateFunc = std::function<std::shared_ptr<IRETask>(ARGS)>;
    void ConfigTask(const KEY& key_, CreateFunc func_); //Configures tasks using a factory method.

    void Stop(); //Stops the reactor by cleaning up epoll-related resource

    void Run(); //Starts the reactor by running the ReactorThread() in a separate thread.

private:
    Factory<IRETask, KEY, ARGS>* m_factory; // pointer to a Factory that creates tasks.

    ThreadPool m_thread_pool; // instance  ThreadPool 

    PlugNPlay m_pnp; // instance PlugNPlay.

    std::thread m_reactor_thread; // thread to run the ReactorThread function.

    std::unordered_map<int ,std::shared_ptr<IInputSrc<KEY, ARGS>>> m_req_srcs_map;  //map is intended to store fd as keys
                                                                    


/*my add*/
    int m_epoll_fd; 
    int m_pipe[2]; 
    int m_flag_for_loop;
    int m_num_events;

    void ReactorThread(); //waits for events using epoll and processes incoming data by creating tasks.
    void AddFdToEpollWatchList(int fd);

};

/*-----------------------------------------------------------------------------
                        inline header
------------------------------------------------------------------------------*/

/***************************    public     ************************************/
//RequestEngine Ctor
template <typename KEY, typename ARGS>
RequestEngine<KEY, ARGS>::RequestEngine(size_t thread_num_, const std::string& pnp_path_)
    :   m_factory(Singleton<Factory<IRETask, KEY, ARGS>>::GetInstance()), 
        m_thread_pool(thread_num_), 
        m_pnp(pnp_path_), 
        m_epoll_fd(epoll_create1(0)), // Create epoll descriptor
        m_flag_for_loop(1) // Set the flag for the loop (assuming 1 means continue)

{
    //add the pipe as a dummy channle to stop the epoll

    if (pipe(m_pipe) == -1) 
    {
        perror("pipe"); // handle error 
    }

    struct epoll_event pipe_epoll_event;
    pipe_epoll_event.events = EPOLLIN;
    pipe_epoll_event.data.fd = m_pipe[0]; 
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_pipe[0], &pipe_epoll_event) == -1) 
    {
        perror("epoll_ctl for pipe"); // handle error 
    }
}

//~RequestEngine() 
template <typename KEY, typename ARGS>
RequestEngine<KEY, ARGS>::~RequestEngine() 
{
    if (1 == m_flag_for_loop) //TODO
    {
        Stop();
    }
}

//ConfigInputSrc - configures input src by adding its fd to the epoll watch list
template <typename KEY, typename ARGS>
void RequestEngine<KEY, ARGS>::ConfigInputSrc(std::shared_ptr<IInputSrc<KEY, ARGS>> input_src_) 
{
    int fd = input_src_->GetFd();
    m_req_srcs_map[fd] = input_src_;
    AddFdToEpollWatchList(fd);
}

//ConfigTask - configures tasks using the factory
template <typename KEY, typename ARGS>
void RequestEngine<KEY, ARGS>::ConfigTask(const KEY& key_, CreateFunc func_) 
{
    m_factory->Add(key_, func_);
}

//Stop() - cleaning up epoll
template <typename KEY, typename ARGS>
void RequestEngine<KEY, ARGS>::Stop() 
{
    char b[20] = "stop the epoll";
    write(m_pipe[1], &b, 20); 

/*m_reactor_thread.join()  consider move to here*/

    // Cleanup epoll and resources
    for (const auto& pair : m_req_srcs_map) 
    {
        epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, pair.first, nullptr);
        // Handle epoll_ctl failure if necessary
    }
    close(m_epoll_fd); 
    close(m_pipe[0]); 
    close(m_pipe[1]); 
}

//Run() - starts the reactor by running the ReactorThread() in a separate thread 
template <typename KEY, typename ARGS>
void RequestEngine<KEY, ARGS>::Run() 
{
    //option1 - pass the address of the ReactorThread() to the std::thread Ctor
    m_reactor_thread = std::thread(&RequestEngine<KEY, ARGS>::ReactorThread, this);

    m_reactor_thread.join(); //blocks main thread until the ReactorThread completes its execution.
}

/***************************    private     ***********************************/
/*  ReactorThread contains the event loop responsible for handling events and 
    processing data from input sources.*/
template <typename KEY, typename ARGS>
void RequestEngine<KEY, ARGS>::ReactorThread() 
{
    const int max_events = 10;
    struct epoll_event events[max_events];
    while (m_flag_for_loop) 
    {
        int num_events = epoll_wait(m_epoll_fd, events, max_events, -1);
        if (num_events == -1) 
        {
            throw std::runtime_error("epoll fail");
        }

        for (int i = 0; i < num_events; ++i) 
        {
            if(events[i].data.fd == m_pipe[0])
            {
                //if read from pipe then stop
                m_flag_for_loop = false;
                break;
            }
            else if(events[i].events & EPOLLIN)
            {
                //if write and ready
                int fd = events[i].data.fd;
                std::pair<KEY, ARGS> ret_read = m_req_srcs_map[fd]->Read(); 
                std::cout << ret_read.first << std::endl;
                m_thread_pool.AddTask(m_factory->Create(ret_read.first, ret_read.second));
            }
        }
    }
}

template <typename KEY, typename ARGS>
void RequestEngine<KEY, ARGS>::AddFdToEpollWatchList(int fd) 
{
    struct epoll_event event;
    event.events = EPOLLIN ; //EPOLLIN: caller wants to receive notification for read events
                                      //EPOLLET: get notifications only when there's a change in the status of the fd
    event.data.fd = fd; // assigns fd -> indicating which fd to monitor

    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) 
    {
        perror("epoll_ctl for adding fd to the watch list");
        // Log error or throw exception if necessary
    }
}

}//namespace hrd31
#endif //__HRD31_RE_FW_HPP__

