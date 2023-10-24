#ifndef __MY_UTILS_HPP__
#define __MY_UTILS_HPP__

/*********************************Macros***************************************/
#ifdef __cplusplus
  #if __cplusplus < 201103L
    #define NOEXCEPT throw()
    #define OVERRIDE
  #else
    #define NOEXCEPT noexcept
    #define OVERRIDE override
  #endif
#endif

#ifdef DEBUG
#define DEBUG_ONLY(code) code
#else
#define DEBUG_ONLY(code)
#endif

#endif /*__MY_UTILS_HPP__*/


