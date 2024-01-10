/* -----------------------------------------------------------------------------
Description: PNP - runtime plug-and-play.
             using the inotify mechanism to monitor a directory for changes and 
             load shared object files dynamically.
             Each class has its own specific responsibility within the 
             plug-and-play system: 

             PlugNPlay  - manages overall functionality 
             DirMonitor - watches the directory for changes. 
             SOLoader   - handles loading of shared objects detected in that 
                          directory.
Author: HRD31
Reviewer: Itzik
Version:
    v0.1 - For mentor approval
    v0.2 - Approved
----------------------------------------------------------------------------- */
#ifndef __HRD31_PLUG_N_PLAY_HPP__
#define __HRD31_PLUG_N_PLAY_HPP__

#include <string> //std::string
#include <memory> //std::unique_ptr
#include<thread> //std::thread

#include "factory.hpp"
#include "msg_broker_imp.hpp"


namespace hrd31
{

/*************************   PlugNPlay    *************************************/

/*  Represents a runtime plug-and-play system that manages the loading and 
    monitoring of shared objects (SOs) at runtime.*/

class PlugNPlay
{
public:
    explicit PlugNPlay(const std::string &path_ = "./pnp"); 
    ~PlugNPlay() = default;

    PlugNPlay(const PlugNPlay &other_) = delete;
    PlugNPlay &operator=(const PlugNPlay &other_) = delete;

private:
    class DirMonitor;
    class SOLoader;

    Dispatcher<std::string> m_dispatcher; /* Manages communication between 
                                             components by dispatching msg 
                                             related to file paths.*/

    std::unique_ptr<DirMonitor> m_monitor; /* Monitors the specified directory 
                                              for changes in SO files.*/

    std::unique_ptr<SOLoader> m_loader; /* Loads the SO files found in the directory.*/
};

/***********************   ::DirMonitor    ************************************/
/* Monitors the specified directory for changes in shared object files.*/

class PlugNPlay::DirMonitor
{
public:
    explicit DirMonitor(Dispatcher<std::string> &dispatcher_,
                        const std::string &path_ = "./pnp");
    ~DirMonitor();

    DirMonitor(const DirMonitor &other_) = delete;
    DirMonitor &operator=(const DirMonitor &other_) = delete;


private:
    static void ListenerThread(DirMonitor *this_, int fd, const std::string &path_);
    int m_dir_fd; /* Represents the directory file descriptor being monitored.*/
    int m_watch_fd;
    Dispatcher<std::string> &m_dispatcher; /* Reference to the associated dispatcher.*/
    std::thread m_async_listener; /*Executes asynchronous operations to monitor 
                                    changes in the directory.*/
};

/*************************   ::SOLoader    ************************************/
/* Loads shared object files discovered in the directory being monitored.*/

class PlugNPlay::SOLoader
{
public:
    explicit SOLoader(Dispatcher<std::string> &dispatcher_);
    ~SOLoader();

    SOLoader(const SOLoader &other_) = delete;
    SOLoader &operator=(const SOLoader &other_) = delete;

private:
    CallBack<std::string, SOLoader> m_call_back; /* Handles the callback 
                                                    functionalities for loading 
                                                    SO files.*/

    void Load(const std::string &file_name_);  /*Manages communication between 
                                                   components by dispatching msg 
                                                   related to file paths.*/
    std::vector<void*> m_handles;
};

} // namespace hrd31

#endif //__HRD31_PLUG_N_PLAY_HPP__
