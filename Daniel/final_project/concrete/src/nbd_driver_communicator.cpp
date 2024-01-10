#include <cassert>     //assert
#include <sys/ioctl.h> //ioctl
#include <sys/socket.h>//socketpair
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h> //O_RDWR
#include <cstring>// strerror
#include <cerrno>
#include <cstdio> //
#include <iostream>
#include <ostream>
#include <linux/nbd.h> //NBD_SET_BLKSIZE 
#include <csignal> //sigaction


#include "nbd_driver_communicator.hpp"

#ifndef BUSE_DEBUG
  #define BUSE_DEBUG (0)
#endif

enum {NBD_SK, APP_SK};

namespace hrd31 
{

/************ initialize static member: m_nbd_fd_disconnect *******************/

//static
volatile std::sig_atomic_t NBDriverCommunicator::m_nbd_fd_disconnect = -1;

/********************** temporary handle for errors ***************************/

void handle_error(const std::string& message)
{
    std::cout << "----------------handle_error----------------" << std::endl;

    std::cerr << "Error: " << message << std::endl;
    // Perform other error handling actions
}

/************* Class FD (file descriptor) private members**********************/

//Ctor for m_nbd_fd
NBDriverCommunicator::FD::FD(const std::string& dev_path_) : m_fd(-1)
{
    m_fd = open(dev_path_.c_str(), O_RDWR);
    std::cout << "m_fd is from FD Ctor: "<< m_fd  << std::endl;

    if (m_fd == -1)
    {
        handle_error("Failed to open NBD device");
        //throw NBDError(); 
    }
}

//Ctor for m_app_socket & m_nbd_socket - only initialize
NBDriverCommunicator::FD::FD(int fd_) : m_fd(fd_) {}

NBDriverCommunicator::FD::~FD()
{
    close(m_fd);
    m_fd = -1;
}

//copy assignment operator=
NBDriverCommunicator::FD& NBDriverCommunicator::FD::operator=(int other_) 
{
    m_fd = other_;
    return *this;
}   

//Implicit conversion from FD to int 
NBDriverCommunicator::FD::operator int() const noexcept
{
    return m_fd;
}


/********************** Private member functions ******************************/
//static
void NBDriverCommunicator::SignalHandlerDisconnectNBD(int signal)
{
     assert(signal == SIGINT || signal == SIGTERM);

     if (m_nbd_fd_disconnect != -1)
     {
        if(ioctl(m_nbd_fd_disconnect, NBD_DISCONNECT) == -1)
        {
            handle_error("SignalHandlerDisconnectNBD: Failed to request disconect on nbd device");
            //throw error
        }
        else
        {
           m_nbd_fd_disconnect = -1;
           fprintf(stderr, "SignalHandlerDisconnectNBD: Successfully requested disconnect on nbd device\n");
        }
     }
}

//Setup (call to ListenerThread)
void NBDriverCommunicator::Setup(const std::string& dev_path, size_t size_,
                                        size_t num_blocks_, size_t block_size_) 
{
    int socket_pair[2]; 

    //AF_UNIX- allow communication between processes on the same machine
    int err = socketpair(AF_UNIX, SOCK_STREAM, 0, socket_pair);
    if (err == -1) 
    {
        handle_error("Failed socketpair\n");
        // throw NBDError();
    }

    m_nbd_socket = socket_pair[NBD_SK];
    m_app_socket = socket_pair[APP_SK];
    m_nbd_fd_disconnect = m_nbd_fd;

                                    std::cout << "setup m_nbd_fd is from FD Ctor: "<< m_nbd_fd  << std::endl;
                                    std::cout << "setup m_nbd_socket is from FD Ctor: "<< m_app_socket  << std::endl;
                                    std::cout << "setup m_app_socket is from FD Ctor: "<< m_nbd_socket  << std::endl;

    if (m_nbd_fd == -1) 
    {
        handle_error("Failed open dev_path_\n");
    }

    if (size_ != 0 && block_size_ == 0 && num_blocks_ == 0) 
    {
        // Set size
        if (ioctl(m_nbd_fd, NBD_SET_SIZE, size_) == -1) 
        {
            handle_error("Failed to set block size");
        }
    } else if (size_ == 0 && block_size_ != 0 && num_blocks_ != 0) 
    {
        // Set block size and number of blocks
        if (ioctl(m_nbd_fd, NBD_SET_BLKSIZE, block_size_) == -1) 
        {
            handle_error("Invalid NBD block size");
        }
        if (ioctl(m_nbd_fd, NBD_SET_SIZE_BLOCKS, num_blocks_) == -1) 
        {
            handle_error("Invalid NBD blocks number");
        }
    } else {
        // Invalid combination of parameters
        handle_error("Invalid combination of size parameters");
    }

    //Clear any data buffered in the socket
    if (ioctl(m_nbd_fd, NBD_CLEAR_SOCK) == -1)
    {
        handle_error("Failed to clear NBD socket");
    }

    // Block all signal so not get interrupted in ioctl(NBD_DO_IT)
    sigset_t sigset;
    if (
       sigfillset(&sigset) != 0 || sigprocmask(SIG_SETMASK, &sigset, NULL) != 0) 
    {
        std::cerr << "failed to block all signals: " 
                                            << std::strerror(errno) << std::endl;
        throw std::runtime_error("Error occurred");
    }

    //ListenerThread
    m_set_clean = std::thread(&NBDriverCommunicator::ListenerThread, this);

    /*Set SIGINT + SIGTERM in SignalHandlerDisconnectNBD */
    struct sigaction act;
    act.sa_handler = SignalHandlerDisconnectNBD;
    act.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &act, nullptr) != 0 || 
                                        sigaction(SIGTERM, &act, nullptr) != 0)
    {
        handle_error("Failed to set SignalHandlerDisconnectNBD");
        //throw NBDError();
        //throw std::runtime_error("Error occurred");
    }   

    printf("Setup end\n");
}

//Listener to reqests (NBD_DO_IT)
void NBDriverCommunicator::ListenerThread() 
{
    int flags;

    /*must static_cast it doesnt convert it implicitly. ioctl can get anything*/
    if (ioctl(m_nbd_fd, NBD_SET_SOCK, static_cast<int>(m_nbd_socket))== -1)
    {
        handle_error("Failed NBD_SET_SOCK");
    } else {
/*
        //Flags 
        #if defined NBD_SET_FLAGS
            flags = 0;
        #if defined NBD_FLAG_SEND_TRIM
            flags |= NBD_FLAG_SEND_TRIM;
        #endif
        #if defined NBD_FLAG_SEND_FLUSH
            flags |= NBD_FLAG_SEND_FLUSH;
        #endif
            if (flags != 0 && ioctl(m_nbd_fd, NBD_SET_FLAGS, flags) == -1){
                fprintf(stderr, "ioct(nbd, NBD_SET_FLAGS, %d) failed.[%s]\n", 
                                                        flags, strerror(errno));
                exit(EXIT_FAILURE);
            }
        #endif
*/
        //Start NBD_DO_IT
        int err = ioctl(m_nbd_fd, NBD_DO_IT);
        if (err == -1) 
        {
            handle_error("NBD_DO_IT failed\n");
            exit(EXIT_FAILURE);
        }

        // clean up
        if (ioctl(m_nbd_fd, NBD_CLEAR_QUE) == -1 ||  
                                        ioctl(m_nbd_fd, NBD_CLEAR_SOCK) == -1)
        {
            handle_error("Failed to perform NBD cleanup actions");
            //exit(EXIT_FAILURE);
        } 
    }

                                std::cout << "ListenerThread end\n";
}

ssize_t NBDriverCommunicator::WriteAll(int fd, const void* buf, size_t count) 
{
    ssize_t bytes_written;
    const char* buffer = static_cast<const char*>(buf);

    while (count > 0) 
    {
        bytes_written = write(fd, buffer, count);
        if (bytes_written < 0) 
        {
            handle_error("Failed to write to file descriptor");
            return -1;  //error
        }

        count -= bytes_written;
        buffer += bytes_written;
    }
    return 0; // Successful write
}

ssize_t NBDriverCommunicator::ReadAll(int fd, void* buf, size_t count) 
{
    ssize_t bytes_read;
    char* buffer = static_cast<char*>(buf);

    while (count > 0) 
    {
        bytes_read = read(fd, buffer, count);
        if (bytes_read < 0) 
        {
            handle_error("Failed to read from file descriptor");
            return -1; // error
        } else if (bytes_read == 0) 
        {
            // Handle EOF or connection closed if needed
            break;
        }

        count -= bytes_read;
        buffer += bytes_read;
    }

    if (count != 0) 
    {
        // Not all bytes were read, handle it accordingly
        handle_error("Not all bytes were read");
        return -1; // error 
    }

    return 0; // Successful read
}


/******************* Public class NBDriverCommunicator ************************/

//Ctor : storage_size
NBDriverCommunicator::NBDriverCommunicator(const std::string& dev_path, 
                                                          size_t storage_size) :
           m_nbd_socket(-1), m_app_socket(-1), m_nbd_fd(dev_path), m_set_clean() 
{
    Setup(dev_path, storage_size, 0, 0);//Set size + clear socket
    std::cout << "NBDriverCommunicator Ctor(storage_size_) end" << std::endl;

}

//Ctor : blocks_num, block_size
NBDriverCommunicator::NBDriverCommunicator(const std::string& dev_path, 
                                         size_t blocks_num, size_t block_size) :
           m_nbd_socket(-1), m_app_socket(-1), m_nbd_fd(dev_path), m_set_clean() 
{
    Setup(dev_path, 0, blocks_num, block_size);//Set size+socket+clear socket
    std::cout << "NBDriverCommunicator Ctor(block_size_ + blocks_num_) end" 
                                                                   << std::endl;
}

//Dtor
NBDriverCommunicator::~NBDriverCommunicator() noexcept 
{
                            std::cout << "Dtor NBDriverCommunicator" << std::endl;
    Disconnect();
                            std::cout << "NBDriverCommunicator Dtor end" << std::endl;
}

//Public member functions
std::shared_ptr<DriverData> NBDriverCommunicator::ReceiveRequest() 
{
    printf("ReceiveRequest start\n");

    nbd_request nbd_req;

            // Read the request from the fd
            //std::cout << "app s " << m_app_socket << std::endl;
    ssize_t bytes_read = ReadAll(m_app_socket, &nbd_req, sizeof(nbd_req)); 
            //std::cout << "bytes_read " << bytes_read << std::endl;
            //std::cout << "sizeof(nbd_req) " << sizeof(nbd_req) << std::endl;

    if (bytes_read == -1) 
    {
        std::cout << bytes_read << " " << sizeof(nbd_req) << std::endl;
        handle_error("Failed to read NBD request");
        //throw an exception or return nullptr
    }

    std::shared_ptr<DriverData> data_nbd(new DriverData(nbd_req));

    if(data_nbd->m_type == WRITE)
    {
        ReadAll(m_app_socket, data_nbd->m_data.data(), data_nbd->m_len );
    }
    printf("ReceiveRequest end\n");

    return data_nbd;
}

void NBDriverCommunicator::SendReply(std::shared_ptr<DriverData> data_) 
{
    printf("SendReply start\n");

    nbd_reply nbd_rep;

    // Fill the reply structure based on data in DriverData object
    nbd_rep.magic = htonl(NBD_REPLY_MAGIC);
    nbd_rep.error = data_->m_status == SUCCESS ? 0 : 1;

    // Copy handle to the reply
    std::memcpy(nbd_rep.handle, &data_->m_handle, sizeof(data_->m_handle));
    std::cout << data_->m_handle << data_->m_handle << std::endl;

    // Send the reply to the NBD 
    ssize_t bytes_sent = write(m_app_socket, &nbd_rep, sizeof(nbd_rep));
    if (bytes_sent == -1) 
    {
        handle_error("Failed to send reply to NBD");
    }

    if(data_->m_type == READ)
    {
        WriteAll(m_app_socket, data_->m_data.data(), data_->m_len);
    }
    printf("SendReply end\n");

}

// Disconnect: may throw bad_close, bad_ioctl, bad_join 
void NBDriverCommunicator::Disconnect() 
{
    if (m_nbd_fd_disconnect != -1)
    {
        m_nbd_fd_disconnect = -1;
        fprintf(stderr, "Disconnect\n");
    
    }

    close(m_app_socket);
    if (m_set_clean.joinable())
    {
        m_set_clean.join();
    }
}

int NBDriverCommunicator::GetRequestFD()
{
    return m_app_socket;
}


} // namespace hrd31


