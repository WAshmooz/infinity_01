/* -----------------------------------------------------------------------------
Description: utils header file for Thread Pool
Author: Daniel
Version:
    v0.1 - DEBUG_ONLY
----------------------------------------------------------------------------- */
#ifndef UTILS_HPP
#define UTILS_HPP

// ANSI color codes for text formatting
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"


#ifdef DEBUG
#define DEBUG_ONLY(code) code
#else
#define DEBUG_ONLY(code)
#endif


#endif // UTILS_HPP

