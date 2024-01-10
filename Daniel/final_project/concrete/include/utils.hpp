/* -----------------------------------------------------------------------------
Description: utils header file
Author: Daniel
Version:
    v0.1 - DEBUG_ONLY
----------------------------------------------------------------------------- */
#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdio> //  printf
#include <iostream> // std::cout

#ifdef DEBUG
#define DEBUG_ONLY(code) code
#else
#define DEBUG_ONLY(code)
#endif // DEBUG

#endif // UTILS_HPP

