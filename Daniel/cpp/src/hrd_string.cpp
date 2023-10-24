/*******************************************************************************
 * Author: Daniel
 * Date: 12/09/2023
 * Version: 1
 * Description: String Source
 *******************************************************************************/
#include <iostream>
#include <cstring> // strlen, strcpy
#include <cassert> // for assert

#include "hrd_string.hpp"

namespace ilrd
{
    //static 
    char *String::AllocAndCpy(const char *src_)
    {
        size_t size = strlen(src_) +1;
        char *ret_str = new(std::nothrow) char[size];
        /*  When std::nothrow is specified, the new operator returns a null 
            pointer if it fails to allocate memory, instead of throwing 
            an exception.*/
        std::copy(src_, src_+size, ret_str);

        return ret_str;
    }

    String::String(const char *user_str_)
    {
        m_str = AllocAndCpy(user_str_);
    }

    // Destructor
    String::~String()
    {
        delete[] m_str;
        m_str = 0;
        DEBUG_ONLY (m_str = NULL_PRT;)
    }

    // Copy constructor
    String::String(const String &o_)
    {
        m_str = AllocAndCpy(o_.m_str);
    }

    // Assignment operator
    String& String::operator=(const String &o_)
    {
        char *temp = AllocAndCpy(o_.Cstr());
        delete[] m_str;
        m_str = temp;

        return *this;
    }

    // Member function to get string length
    size_t String::Length() const
    {
        assert(m_str);
        return strlen(m_str);
    }

    // Member function to get C-style string
    const char *String::Cstr() const
    {
        return m_str;
    }

}//namespace ilrd
