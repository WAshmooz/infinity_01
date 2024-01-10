/*
Description:
Author:
Reviewer:
Version:
    v0.1 - For mentor approval
*/

#ifndef __HRD31_NBD_DRIVER_COMMUNICATOR_HPP__
#define __HRD31_NBD_DRIVER_COMMUNICATOR_HPP__

#include <string>
#include <thread> //thread
#include <stdexcept>
#include <csignal> //sig_atomic_t

#include "i_driver_communicator.hpp"
#include "driver_data.hpp"

namespace hrd31
{
class DriverError;

class NBDriverCommunicator : public IDriverCommunicator
{
public:
    explicit NBDriverCommunicator(const std::string& dev_path_,
            size_t storage_size_); //may throw bad_open, bad_read, bad_signal, bad_ioctl
    explicit NBDriverCommunicator(const std::string& dev_path_, size_t blocks_num_, size_t block_size_);
    ~NBDriverCommunicator() noexcept override;

    //Block CCtor + Assignment operator=
    NBDriverCommunicator(const NBDriverCommunicator& other_) = delete;
    NBDriverCommunicator& operator=(const NBDriverCommunicator& other_) = delete;
    
    std::shared_ptr<DriverData> ReceiveRequest() override; //may throw bad_read
    void SendReply(std::shared_ptr<DriverData> data_) override; //may throw bad_write
    void Disconnect() override; //may throw bad_close, bad_ioctl, bad_join 
    int GetRequestFD() override;

    // Signal handler to gracefully disconnect from nbd kernel driver.
    static volatile std::sig_atomic_t m_nbd_fd_disconnect; 

private:
    class FD
    {
    public:
        FD(int fd_);//Ctor for m_nbd_socket & m_app_socket
        FD(const std::string& dev_path_); //Ctot for m_nbd_fd;
        ~FD();//Dtor

        FD(const FD& ) = delete; //no Cctor
        FD& operator=(int other_); //copy assingment operator

        operator int() const noexcept;
     private:

        int m_fd;
    };

    FD m_nbd_socket;
    FD m_app_socket;
    FD m_nbd_fd;
    std::thread m_set_clean;

    // private member functions
	void ListenerThread();
    void Setup(const std::string& dev_path_, size_t size_ = 0,
			   size_t num_blocks_ = 0, size_t block_size_ = 0);
    ssize_t WriteAll(int fd, const void* buf, size_t count);
    ssize_t ReadAll(int fd, void* buf, size_t count);

    static void SignalHandlerDisconnectNBD(int signal);
    
};

}//namespace hrd31

#endif //__HRD31_NBD_DRIVER_COMMUNICATOR_HPP__

