/*
Description: Synchronous logger for multithreaded environment
Author: HRD31
Reviewer: Itzik
Version:
    v0.1 - For mentor approval
    v0.1.1 - Mentor approval
*/
#ifndef __ILRD_HRD31_LOGGER__
#define __ILRD_HRD31_LOGGER__

#include <string>   //std::string
#include <fstream>  //std::ofstream
#include <iostream>

#include "singleton.hpp"

namespace hrd31
{

class Logger
{

public:
    typedef enum
    {
        ERROR = 0,
        INFO,
        DEBUG
    } log_level_t;

    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void SetLogLevel(log_level_t level_);
    void SetFilePath(const std::string& path_to_file_ = "./log");

    void Log(   log_level_t level_,
                const std::string& msg_, 
                const std::string& file_, 
                int line_,  
                bool print_outstream_ = false);

private:
    std::ofstream m_stream;
    log_level_t m_log_level;
    std::mutex m_mutex; // added mutex for thread safety

    Logger(); //private for collaboration with singleton
    friend class Singleton<Logger>;
};

}//namespace hrd31



#endif /* __ILRD_HRD31_LOGGER__ */