/* -----------------------------------------------------------------------------
Description:
Author: HRD31
Reviewer: Itzik
Version:
    v0.1 - For mentor approval
    v0.2 - Approved
----------------------------------------------------------------------------- */
#include <dlfcn.h> // dlopen, dlerror, RTLD_LAZY
#include <unistd.h> // read, close
#include <errno.h>
#include <sys/inotify.h> //inotify
#include <iostream> // std::cout
#include <string> //std::string
#include <thread> //std::thread
#include <memory> //std::unique_ptr


//#include <stdio.h>
//#include <stdlib.h>
//#include <gnu/lib-names.h>
//#include <poll.h>
//#include <cstring>

#include "pnp.hpp"

namespace hrd31
{

    enum
    {
        MAX_BUF_SIZE = 1024
    };

/*************************   PlugNPlay    *************************************/
   
PlugNPlay::PlugNPlay(const std::string &path_)
    :   m_dispatcher(),
        m_monitor(new DirMonitor(m_dispatcher, path_)),
        m_loader(new SOLoader(m_dispatcher))
{
}

// PlugNPlay::~PlugNPlay() {}

/***********************   ::DirMonitor    ************************************/
// static - monitoring changes in the specified directory.
/*
    Watch on the fd and read it (when created) to the buff. in the Loop 
    monitoring file system events using the inotify mechanism in Linux.
    NotifyAll if there where file system event (create)
*/
void PlugNPlay::DirMonitor::ListenerThread(DirMonitor *this_, int fd, 
                                            const std::string &path_)
{
    char buf[MAX_BUF_SIZE] = {0};

    this_->m_watch_fd = inotify_add_watch(fd, path_.c_str(), IN_CLOSE_WRITE);

    std::cout << "path: " << path_.c_str() << std::endl;                 //DEBUG
    std::cout <<"Listening for events."<< std::endl;                     //DEBUG

    /*  reads events from the inotify fd to the buffer. It blocks until there 
        are events to read or until an error occurs (256 = max_file_name_len).*/
    while (0 < read(fd, buf, 256 + sizeof(struct inotify_event))) 
    {
        std::cout <<"Listening Start loop"<< std::endl;
        inotify_event *event = (inotify_event *)buf;
        std::cout << event->name << std::endl;                           //DEBUG
        //sleep(1);   // change it with checking IN_CLOSE also in inotify
        this_->m_dispatcher.NotifyAll(event->name);
    }

    std::cout << "ListenerThread() End" << std::endl;                    //DEBUG
}

//Ctor
PlugNPlay::DirMonitor::DirMonitor(Dispatcher<std::string> &dispatcher_,
                                    const std::string &path_)
    :   m_dir_fd(inotify_init()),
        m_dispatcher(dispatcher_),
        m_async_listener(ListenerThread, this, m_dir_fd, path_)
{
}

//Dtor
PlugNPlay::DirMonitor::~DirMonitor()
{
    inotify_rm_watch(m_dir_fd, m_watch_fd);
    close(m_dir_fd);
    m_async_listener.join(); //can throw exception (move to StopFun() ??)
    std::cout << "End ~DirMonitor()" << std::endl;
}

/*************************   ::SOLoader    ************************************/
//Ctor
PlugNPlay::SOLoader::SOLoader(Dispatcher<std::string> &dispatcher_)
    : m_call_back(dispatcher_, *this, &PlugNPlay::SOLoader::Load)
{
}    
/*  
    The `CallBack` class is initialized 
    This initialization will automatically register the `Load` member 
    function as a callback within the dispatcher for the associated message 
    type.
*/

//Dtor
PlugNPlay::SOLoader::~SOLoader()
{
    for (auto handle : m_handles)
    {
        dlclose(handle);
    }
    std::cout << "after for so loader" << std::endl;
}

// notify
void PlugNPlay::SOLoader::Load(const std::string &file_name_)
{
    std::cout << file_name_.c_str() << std::endl;
    /*  we fixed the flags to RTLD_NOW | RTLD_GLOBAL ensuring immediate 
        symbol resolution and making the symbols available for subsequent 
        libraries that are loaded.*/
    void* handle = dlopen(file_name_.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if(NULL == handle)
    {
        std::cout << "dlopen failed" << std::endl;
        std::cerr << "dlopen failed: " << dlerror() << std::endl;
    //error handling if m_handle is nullptr
    } else{
        m_handles.push_back(handle); // Store the handle in the vector
    }
}

} // namespace hrd31


/*
Errors:
    -g++ -std=c++11 -shared -fPIC ../test/pnp_lib.cpp -o libsample.so
     if we create the .so inside the directory on pnp we will get error becuase 
     it try to open (dlopen) as it started to create it and does not wait until 
     it finish to create the file. 
     Sulotion: create it in the freamwork directory (or any other directory)
     and then copy it:  cp libsample.so ./pnp/libsample.so

*/ 
