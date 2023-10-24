/*******************************************************************************
* Author: Daniel
* Date: 12/09/2023
* Version: 1
* Description: String Include
*******************************************************************************/
#ifndef __HRD_STRING_H__
#define __HRD_STRING_H__

#include <cstddef> //size_t
#include <iostream> //ostream

#include <cstring> //strlen, strcpy

#include "my_utils.hpp" //NOEXCEPT

/******************************************************************************/

namespace ilrd 
{

class String;

std::ostream& operator<<(std::ostream& os_, const String& str);
/*String myString("Hello, world!");
std::cout << myString; 
Uses the operator<< function to print myString to the standard output.*/

bool operator==(const String& str1, const String& str2);
bool operator!=(const String& str1, const String& str2);
bool operator<(const String& str1, const String& str2);
bool operator>(const String& str1, const String& str2);

bool operator==(const char *str1, const String& str2);

class String
{
public:

    // No defult Ctor
    explicit String(const char *str_); 
    String(const String &o_); 
    ~String(); 

    String &operator=(const String &o_); // Assignment operator
    
    //define Length as member provides an object-oriented user experience
    size_t Length() const;

    //null terminator
    const char *Cstr() const;

private:
    char *m_str;

    static char *AllocAndCpy(const char *src_);

};

} // namespace ilrd

#endif    /*__HRD_STRING_H__*/

