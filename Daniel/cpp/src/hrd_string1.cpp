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

using namespace std;
using namespace ilrd;

class String
{
public:

    explicit String(const char *str_)
    {
        m_str = new char[strlen(str_) + 1];
        strcpy(m_str, str_);
    }

    // Copy constructor
    String(const String &o_)
    {
        m_str = new char[strlen(o_.m_str) + 1];
        strcpy(m_str, o_.m_str);
    }

    // Assignment operator
    String &operator=(const String& o_)
    {
        char *temp = new char[strlen(o_.m_str) + 1];
        strcpy(temp, o_.m_str);
        delete[] m_str;
        m_str = temp;

        return *this;
    }

    // Destructor
    ~String()
    {
        delete[] m_str;
        //DEBUG_ONLY (m_str = NULL_PRT;)
    }

    inline size_t Length() const
    {
        assert(m_str);
        return strlen(m_str);
    }

    inline const char *Cstr() const
    {
        return m_str;
    }

private:
    char *m_str;
};


