/*******************************************************************************
****** Author: Daniel 
****** Date: 22.10.2013
****** Version: 
****** Description: RC String Include
*******************************************************************************/
#ifndef __RC_STRING_HPP__
#define __RC_STRING_HPP__

#include <cstddef> 		//size_t 

#include "my_utils.hpp" //NOEXCEPT

namespace ilrd
{

class String;

bool operator==(const String& str1_, const String& str2_);

bool operator!=(const String& str1_, const String& str2_);

bool operator>(const String& str1_, const String& str2_);

bool operator<(const String& str1_, const String& str2_);


class String
{
public:
	String(const char * str_ = ""); //No explicit on purpose

 	~String() NOEXCEPT;

	String(const String& o_);

	String& operator=(const String& o_);

	size_t Length() const;

	const char *ToCStr() const;

    char operator[](size_t idx_) const;

	char& operator[](size_t idx_);
 
private:
	
	class SharedBuffer;

    SharedBuffer *m_ref_buffer; 
	static SharedBuffer *InitNewSharedBufIMP(const char *str_);
};



} //namespace ilrd

#endif	//__RC_STRING_HPP__
