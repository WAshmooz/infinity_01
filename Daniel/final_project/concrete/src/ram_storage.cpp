#include <cstring> // memcpy
#include <stdexcept> // exceptions
#include <iostream>

//#include "i_storage.hpp"
#include "ram_storage.hpp"

namespace hrd31 
{

RAMStorage::RAMStorage(size_t size_) : IStorage(size_), m_storage(size_)
{

}

RAMStorage::~RAMStorage() noexcept {}

RAMStorage::RAMStorage(const RAMStorage& other_) : 
               IStorage(other_.m_storage.size()), m_storage(other_.m_storage) {}

void RAMStorage::Read(std::shared_ptr<DriverData> read_data_) const
{
    // Check if the read operation (offset + len) goes beyond the storage size
    if (read_data_->m_offset + read_data_->m_len > m_storage.size()) 
    {
        std::cout << "Error: RAMStorage Read" << std::endl;
        throw std::out_of_range("Error: RAMStorage Read - Attempt to read beyond storage size");
    }

    // Copy data from storage to DriverData's buffer.
    //copy (src_start, src_end, dest_start) ,   src = storage , dest = buffer
    std::copy(m_storage.begin() + read_data_->m_offset,
              m_storage.begin() + read_data_->m_offset + read_data_->m_len,
              (read_data_->m_data).begin());
}


void RAMStorage::Write(std::shared_ptr<DriverData> write_data_)
{
    // Check if the write operation goes beyond the storage size
    if ( (write_data_->m_offset) + (write_data_->m_len) > m_storage.size() ) 
    {
        std::cout << "Error: RAMStorage Write" << std::endl;
        throw std::out_of_range("Error: RAMStorage Write - Attempt to write beyond storage size");

    }
    
    // Copy data from DriverData's buffer to storage.
    //copy (src_start, src_end, dest_start) ,   src = buffer , dest = storage
    std::copy((write_data_->m_data).begin(),
              (write_data_->m_data).begin() + write_data_->m_len,
               m_storage.begin() + write_data_->m_offset);

}

} // namespace hrd31

