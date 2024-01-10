/*
Description:  logger source file
Author: Daniel
Reviewer: 
Version:
    v1 - test the logger
*/
#include <iostream>
#include "logger.hpp"

int main() 
{
    hrd31::Logger* logger = Singleton<hrd31::Logger>::GetInstance();

    // Set log file path and log level
    logger->SetFilePath("./log.txt"); 
    logger->SetLogLevel(hrd31::Logger::DEBUG);

    // Log some messages (only DEBUG set to true)
    logger->Log(hrd31::Logger::INFO, "This is an information message", 
                                                            __FILE__, __LINE__);
    logger->Log(hrd31::Logger::ERROR, "This is an error message", 
                                                            __FILE__, __LINE__);
    logger->Log(hrd31::Logger::DEBUG, "This is a debug message", 
                                                      __FILE__, __LINE__, true);

    return 0;
}
