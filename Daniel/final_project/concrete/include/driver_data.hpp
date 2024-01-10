/*
Description:
Author:
Reviewer:
Version:
    v0.1 - For mentor approval
*/
#ifndef __HRD31_DRIVER_DATA_HPP__
#define __HRD31_DRIVER_DATA_HPP__

#include <cstddef> //size_t
#include <linux/nbd.h>
#include <vector>
#include <netinet/in.h> // For htonl and ntohl

namespace hrd31
{
typedef enum {READ, WRITE, STOP} req_type_t;
typedef enum {SUCCESS, FAIL} status_t;

#ifdef WORDS_BIGENDIAN
u_int64_t ntohll(u_int64_t a) {
  return a;
}
#else
inline u_int64_t ntohll(u_int64_t a) 
{
  u_int32_t lo = a & 0xffffffff;
  u_int32_t hi = a >> 32U;
  lo = ntohl(lo);
  hi = ntohl(hi);
  return ((u_int64_t) lo) << 32U | hi;
}
#endif
#define htonll ntohll 


struct DriverData
{
    explicit DriverData(nbd_request& request_); //may throw bad_alloc, bad_read
    ~DriverData() = default;
    DriverData(const DriverData& other_) = delete;
    DriverData& operator=(const DriverData& other_) = delete;

    req_type_t m_type;
    size_t m_handle;
    size_t m_offset;
    size_t m_len;
    status_t m_status;

    std::vector<char> m_data;
};

inline DriverData::DriverData(nbd_request& request_) :
    m_type(req_type_t(ntohl(request_.type))),
    m_handle((*reinterpret_cast<size_t*>(request_.handle))),
    m_offset(ntohll(request_.from)),
    m_len(ntohl(request_.len)),
    m_status(SUCCESS), // Assuming a default status is SUCCESS
    m_data(m_len)
{}

}//namespace hrd31

#endif //__HRD31_DRIVER_DATA_HPP__
