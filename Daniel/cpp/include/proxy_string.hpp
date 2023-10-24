/*******************************************************************************
 * author:	HRD31
 * reviewer:	-
 * update:	-
 *******************************************************************************/
#ifndef __ILRD_HRD31_RC_STRING__
#define __ILRD_HRD31_RC_STRING__

#include <cstddef> // size_t

#include "my_utils.hpp" // NOEXCEPT

namespace ilrd
{
	class String;

	bool operator==(const String &str1_, const String &str2_);
	bool operator!=(const String &str1_, const String &str2_);

	bool operator<(const String &str1_, const String &str2_);
	bool operator>(const String &str1_, const String &str2_);

	class String
	{
	public:
		String(const char *str_ = "");		 // Ctor no explicit on purpose
		String(const String &o_);			 // CCtor
		String &operator=(const String &o_); // Assignment
		~String() NOEXCEPT;					 // Dtor

		char operator[](size_t idx_) const;
		CharProxy &operator[](size_t idx_);

		size_t Length() const;
		const char *ToCstr() const;

	private:
		class SharedBuffer;
		class CharProxy;

		SharedBuffer *m_shared_buffer;

		static SharedBuffer *NewBufferIMP(const char *);


	}; // String end

} // namespace ilrd

#endif /* __ILRD_HRD31_RC_STRING__ */