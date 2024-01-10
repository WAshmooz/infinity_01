/*
Description:  logger source file
Author: Daniel
Reviewer: 
Version:
    v1 - implemented all
*/

#include "logger.hpp"

hrd31::Logger::~Logger()
{
    m_stream.close();
}

void hrd31::Logger::SetLogLevel(log_level_t level_)
{
    m_log_level = level_;
}

void hrd31::Logger::SetFilePath(const std::string& path_to_file_)
{
    /*  opens the file path_to_file_ in output mode for writing and appends 
        new data to the end of the file if it already exists. If the file 
        doesn’t exist, it creates a new one. */
    m_stream.open(path_to_file_, std::ios::out | std::ios::app);

    if (!m_stream.is_open()) // If the file opening fails
    {
        // error message is printed to the standard error stream
        std::cerr << "Error: Unable to open file: " << path_to_file_ << std::endl;
    }
}

void hrd31::Logger::Log(log_level_t level_,
                 const std::string& msg_,
                 const std::string& file_,
                 int line_,
                 bool print_outstream_)
{
    if (level_ <= m_log_level)
    {
        std::string level_str;
        switch (level_)
        {
            case ERROR:
                level_str = "ERROR";
                break;
            case INFO:
                level_str = "INFO";
                break;
            case DEBUG:
                level_str = "DEBUG";
                break;
            default:
                level_str = "UNKNOWN";
                break;
        }

        // "output" will hold the formatted log message
        std::string output = "[" + level_str + "] " + msg_ +
                             " (Path: " + file_ + ", Line: " + std::to_string(line_) + ")\n";

        if (print_outstream_)   // if flag on
        {
            std::cout << output; //prints the Log msg to the standard output 
        }

        //Lock the mutex ensuring one thread at a time can write to the log file
        std::lock_guard<std::mutex> lock(m_mutex); 
        if (m_stream.is_open()) // if file is open
        {
            m_stream << output; //It writes the Log msg to the file m_stream
            m_stream.flush(); // ensures any buff data written to file immediately.
        }
    }// releasing mutex when out of scope 
}

hrd31::Logger::Logger()
{
    m_log_level = ERROR;
    m_stream.open("./log", std::ios::out | std::ios::app);
}



