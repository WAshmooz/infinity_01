/*
    author: rayan
    rc_string
*/

#include <cstddef> // size_t, offsetof
#include <cstring> // strcmp
#include <cassert>
#include <algorithm> // std::copy

#include "rc_string.hpp"
#include "my_utils.hpp" // NOEXCEPT

namespace ilrd
{

    // ProxyChar  ----------------------------------------------------------
    class String::ProxyChar
    {

    public:
        explicit ProxyChar(String *str_, size_t idx_);

        char & operator=(const ProxyChar&); // copy on write
        
        char& operator=(char c); // copy on write
        char* operator&();       // copy on write

        operator char();

    private:
        static String *SplitStringIMP(const String *);
        char* StringPtr();
        
        String *str_handle;
        size_t m_idx;    
    };

    char* String::ProxyChar::StringPtr()
    {
        str_handle->m_shared_buffer.Buffer();
    }

    String::ProxyChar(String *str_, size_t idx_) : str_handle(str_), idx(idx_) {}

    char &String::ProxyChar::operator=(char c)
    {
        SplitStringIMP(str_handle);
        StringPtr()[m_idx] = c;
    }

    char *String::ProxyChar::operator&()
    {
        return StringPtr() + m_idx;
    }

    String::ProxyChar::operator char()
    {
        return StringPtr()[m_idx];
    }

    // static
    String String::ProxyChar::SplitStringIMP(const String *o_)
    {
        String::SharedBuffer *new_buffer = NewBufferIMP(ToCstr());
        
        m_shared_buffer->DetachNDestroy(); // does not throw
        m_shared_buffer = new_buffer;
        m_shared_buffer->Attach(); // does not throw
    }

    // SharedBuffer ----------------------------------------------------------
    class String::SharedBuffer
    {
    public:
        static SharedBuffer *AllocSharedBuffer(size_t size_);

        void Attach() NOEXCEPT;
        void DetachNDestroy() NOEXCEPT;
        bool IsSinglyAttached() const;
        char *Buffer();

    private:
        SharedBuffer();
        ~SharedBuffer() NOEXCEPT;

        SharedBuffer(const SharedBuffer &);            // disable CCtor
        SharedBuffer &operator=(const SharedBuffer &); // disable assign operator

        size_t m_reference_count;
        char m_buffer[1];

    }; // SharedBuffer end

    String::SharedBuffer::SharedBuffer() : m_reference_count(0)
    {
    }

    String::SharedBuffer::~SharedBuffer() NOEXCEPT
    {
    }

    // static
    String::SharedBuffer *String::SharedBuffer::AllocSharedBuffer(size_t size_)
    {
        void *mem = operator new(offsetof(SharedBuffer, m_buffer) + size_);
        return new (mem) SharedBuffer();
    }

    void String::SharedBuffer::Attach() NOEXCEPT
    {
        ++m_reference_count;
    }

    void String::SharedBuffer::DetachNDestroy() NOEXCEPT
    {
        --m_reference_count;

        if (0 == m_reference_count)
        {
            delete this;
            DEBUG_ONLY(this = NULL_PTR);
        }
    }

    bool String::SharedBuffer::IsSinglyAttached() const
    {
        return 1 == m_reference_count;
    }

    char *String::SharedBuffer::Buffer()
    {
        return m_buffer;
    }
    /****************************************************************************/

    bool operator==(const String &str1_, const String &str2_)
    {
        return !strcmp(str1_.ToCstr(), str2_.ToCstr());
    }

    bool operator!=(const String &str1_, const String &str2_)
    {
        return !(str1_ == str2_);
    }

    bool operator<(const String &str1_, const String &str2_)
    {
        return 0 < strcmp(str1_.ToCstr(), str2_.ToCstr());
    }

    bool operator>(const String &str1_, const String &str2_)
    {
        return str2_ < str1_;
    }

    // static
    String::SharedBuffer *String::NewBufferIMP(const char *str_)
    {
        size_t size = strlen(str_) + 1;

        String::SharedBuffer *new_buffer = SharedBuffer::AllocSharedBuffer(size);
        std::copy(str_, str_ + size, new_buffer->Buffer());

        return new_buffer;
    }

    // ctor
    String::String(const char *str_) : m_shared_buffer(NewBufferIMP(str_))
    {
        m_shared_buffer->Attach(); // does not throw
    }

    // cctor
    String::String(const String &o_) : m_shared_buffer(o_.m_shared_buffer)
    {
        o_.m_shared_buffer->Attach(); // does not throw
    }

    String::~String() NOEXCEPT
    {
        m_shared_buffer->DetachNDestroy(); // does not throw
    }

    String &String::operator=(const String &o_)
    {
        // handles self assignment

        o_.m_shared_buffer->Attach();      // does not throw
        m_shared_buffer->DetachNDestroy(); // does not throw
        m_shared_buffer = o_.m_shared_buffer;

        return *this;
    }

    char String::operator[](size_t idx_) const
    {
        assert(idx_ < Length());

        return ToCstr()[idx_];
    }

    // for readonly we return charproxy
    CharProxy String::operator[](size_t idx_)
    {
        assert(idx_ < Length());

        return CharProxy(this, idx_); 

        // if (!m_shared_buffer->IsSinglyAttached())
        // {
        //     String::SharedBuffer *new_buffer = NewBufferIMP(ToCstr());

        //     m_shared_buffer->DetachNDestroy(); // does not throw

        //     m_shared_buffer = new_buffer;

        //     m_shared_buffer->Attach(); // does not throw
        // }

        // return m_shared_buffer->Buffer()[idx_];
    }

    size_t String::Length() const
    {
        return strlen(m_shared_buffer->Buffer());
    }

    const char *String::ToCstr() const
    {
        return m_shared_buffer->Buffer();
    }

} // namespace ilrd
