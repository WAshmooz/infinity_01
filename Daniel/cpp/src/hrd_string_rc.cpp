/*******************************************************************************
*  Author: Daniel Shabso
*  Reviewer:
*  Date: 22.10.2013
*  Version: 1
*  Description: RC String Source
*******************************************************************************/
#include <cassert> 		//assert
#include <cstddef> 		//size_t 
#include <cstring> 		//strlen
#include <algorithm> 	//std::copy
#include <new>			//new()

#include "hrd_string_rc.hpp"

namespace ilrd
{


class String::SharedBuffer
{
public:
	//call the function DetachNDestroy() at the end of use
	static SharedBuffer *AllocSharedBuffer(size_t size_);

	void Attach() NOEXCEPT;

	void DetachNDestroy() NOEXCEPT;

	bool IsSinglyAttached() const;

	char *Buffer();

private:
	//The counter value is initialized to zero to make it easier to trace the 
	//sequence of actions in the code, when a new line or a new reference is 
	//created or a line is opened from the current line, the user is 
	//responsible for calling the appropriate function

	SharedBuffer(); //constructor for internal use only
	~SharedBuffer() NOEXCEPT; //

	SharedBuffer(const SharedBuffer&); //disable CCtor
	SharedBuffer& operator=(const SharedBuffer&); //disable assignment operator

    size_t m_ref_count;
	char m_buffer[1]; //variable size array
};



//class String

String::String(const char * str_) : m_ref_buffer(InitNewSharedBufIMP(str_))
{
	//increase the value of the reference counter
	m_ref_buffer->Attach();
}


String::~String() NOEXCEPT
{
	//decrease the reference counter(the object will be deleted when references
	//count reach 0)
	m_ref_buffer->DetachNDestroy();
}


String::String(const String& o_) : m_ref_buffer(o_.m_ref_buffer)
{	//init the ptr to shared buffer with an address pointed to by another object
	
	//increase the value of the reference counter
	m_ref_buffer->Attach();
}


String& String::operator=(const String& o_)
{
	//handles assignment to self

	//increase the value of the reference counter
	o_.m_ref_buffer->Attach();

	//get buffer of another String
	SharedBuffer *new_buffer = o_.m_ref_buffer;

	//decrease the value of the old SharedBuffer object references count or 
	//delete it if the received object is the only reference for this object
	m_ref_buffer->DetachNDestroy();

	//set current SharedBuffer pointer points to the new SharedBuffer object
	m_ref_buffer = new_buffer;

	return *this;
}


size_t String::Length() const 
{
	// return len of the string strlen()
	return strlen(ToCStr());
}


const char *String::ToCStr() const
{
	//return the retrieved buffer
	return m_ref_buffer->Buffer();
}


char String::operator[](size_t idx_) const
{
	//assert that index is not bigger
	assert(idx_ < strlen(ToCStr()));

	// return the element of array by index 
	return ToCStr()[idx_];
}


char& String::operator[](size_t idx_)
{
	//assert that index is not bigger 
	assert(idx_ < strlen(ToCStr()));

//check if there are other references to current String(reference counter value is bigger than 1)
	if (!m_ref_buffer->IsSinglyAttached())
	{
		//allocate and init the new SharedBuffer
		SharedBuffer *new_buffer = InitNewSharedBufIMP(ToCStr());

		//decrease the value of the old SharedBuffer object references count
		m_ref_buffer->DetachNDestroy();

		//set current SharedBuffer pointer points to the new SharedBuffer object
		m_ref_buffer = new_buffer;

		//increase the value of the reference counter
		m_ref_buffer->Attach();
	}

	// return the element of array by index
	return m_ref_buffer->Buffer()[idx_]; 
}


//static
String::SharedBuffer *String::InitNewSharedBufIMP(const char *str_)
{
	assert(str_);

	//calc size of the string strlen + 1
	size_t size = strlen(str_) + 1;

	//allocate ref buffer by calling AllocSharedBuffer function
	String::SharedBuffer *new_buffer = String::SharedBuffer::AllocSharedBuffer(size);

	//copy received string to buffer std::copy
	std::copy(str_, str_ + size, new_buffer->Buffer());

	return new_buffer;
}


bool operator==(const String& str1_, const String& str2_)
{
	return !strcmp(str1_.ToCStr(), str2_.ToCStr());
}


bool operator!=(const String& str1_, const String& str2_)
{
	return !(str1_ == str2_);
}

bool operator>(const String& str1_, const String& str2_)
{
	return 0 < strcmp(str1_.ToCStr(), str2_.ToCStr());
}

bool operator<(const String& str1_, const String& str2_)
{
	return str2_ > str1_;
}


//struct SharedBuffer


String::SharedBuffer::SharedBuffer() : m_ref_count(0) 
{}

String::SharedBuffer::~SharedBuffer() NOEXCEPT
{}

//static 
String::SharedBuffer *String::SharedBuffer::AllocSharedBuffer(size_t size_)
{
	void *mem_pool = operator new(offsetof(SharedBuffer, m_buffer) + size_);

	return new(mem_pool) SharedBuffer();
}


void String::SharedBuffer::Attach() NOEXCEPT
{
	++m_ref_count;
}

void String::SharedBuffer::DetachNDestroy() NOEXCEPT
{
	--m_ref_count;

	if (!m_ref_count)
	{
		delete this;
		DEBUG_ONLY(this = NULL);
	}
}

bool String::SharedBuffer::IsSinglyAttached() const
{
	return 1 == m_ref_count;
}

char *String::SharedBuffer::Buffer()
{
	return m_buffer;
}



} //namespace ilrd
