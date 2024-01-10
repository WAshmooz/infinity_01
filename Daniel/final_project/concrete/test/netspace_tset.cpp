#include <iostream>
#include <ostream>
#include <cstdlib> // malloc and free
#include <cstring> //memcpy
#include <unistd.h>
#include <sys/epoll.h>

#include "nbd_driver_communicator.hpp"
#include "ram_storage.hpp"

#define DEBUG // Define DEBUG to enable debug mode
#include "utils.hpp"


const int max_num_fd  = 10; //max number of file descriptors
const int size_buf = 10;

int main() 
{
    std::string path = "/dev/nbd1";
    size_t storage_size = 500 * 4096;

try
{
    // Create an instance of NBDriverCommunicator object 
    hrd31::NBDriverCommunicator nbd_driv_coomm(path, storage_size);
    hrd31::RAMStorage ram_storage(storage_size);

    DEBUG_ONLY(std::cout << "End Creating an instance of NBDriverCommunicator object + RAMStorage" << std::endl;)

    // initialize for epoll
    int ep_fd, ret;
    struct epoll_event ep_ev;
    struct epoll_event evlist[max_num_fd];

    // epoll_create
    ep_fd = epoll_create1(0);
    if (0 > ep_fd) //TODO: exceptions
    { 
      perror("epoll_create1"); 
      exit(1); 
    }

    //input
    ep_ev.events = EPOLLIN;

    ep_ev.data.fd = STDIN_FILENO; // fd to read from STDIN (for quit: Q)
    ret = epoll_ctl(ep_fd, EPOLL_CTL_ADD, STDIN_FILENO, &ep_ev); //set epoll_ctl to zeroes
    if (0 > ret)//TODO: exceptions
    { 
      perror("epoll_ctl STDIN_FILENO");
      exit(1);
    }
    
    // app_fd
    ep_ev.data.fd = nbd_driv_coomm.GetRequestFD(); // fd app
    ret = epoll_ctl(ep_fd, EPOLL_CTL_ADD, nbd_driv_coomm.GetRequestFD(), &ep_ev); //add fd to epoll
    if (0 > ret)//TODO: exceptions
    { 
      perror("epoll_ctl m_app_socket");
      exit(1);
    }

    while (-1 != hrd31::NBDriverCommunicator::m_nbd_fd_disconnect)
    {
         DEBUG_ONLY(std::cout << "START LOOP" <<std::endl;)
        // epoll_wait func returns the numb of fd ready for I/O.
        int num_fd_ready = epoll_wait(ep_fd, evlist, max_num_fd, -1);
        if (num_fd_ready == -1)
        { perror("epoll_wait"); 
          exit(1); 
        }

        for (int i = 0; i < num_fd_ready; ++i)
        {
            // get input user to buff and check if its "q" -> Disconnect
            if (evlist[i].data.fd == STDIN_FILENO)
            {
                DEBUG_ONLY(std::cout << "IF STDIN_FILENO" << std::endl;)
                char buf[size_buf];
                fgets(buf, size_buf, stdin);
                if (!strncmp(buf, "q", 1)) //TODO: if (std::string(buf) == "q")
                {
                    DEBUG_ONLY(std::cout << "IF STDIN_FILENO" << std::endl;)
                    nbd_driv_coomm.Disconnect();
                    break;
                }
            }

            // handle requests if you didnt get "q"
            else if(evlist[i].data.fd == nbd_driv_coomm.GetRequestFD())
            {
                // Receive a request
                std::shared_ptr<hrd31::DriverData> request = 
                                                    nbd_driv_coomm.ReceiveRequest();
                
                if (!request) 
                {
                    // Handle null request or break the loop
                    DEBUG_ONLY(std::cout << "request = null" <<std::endl;)
                    break;
                }

            DEBUG_ONLY(
                            // Print the received request data
                    std::cout << "Print the received request data:" << std::endl;
                    std::cout << "Type: " << (request->m_type) << std::endl;
                    std::cout << "Handle: " << request->m_handle << std::endl;
                    std::cout << "Offset: " << request->m_offset << std::endl;
                    std::cout << "Length: " << request->m_len << std::endl;
                )


                switch (request->m_type) 
                {
                    case hrd31::READ: 
                    {
                        DEBUG_ONLY(std::cout<< "switch: READ request"<< std::endl;)
                        ram_storage.Read(request);
                        break;
                    }
                    case hrd31::WRITE: 
                    {
                        DEBUG_ONLY(std::cout<< "switch: WRITE request"<< std::endl;)
                        ram_storage.Write(request);
                        break;
                    }
                    default:
                        DEBUG_ONLY(std::cout<< "switch: default"<< std::endl;)
                        break;
                }

                // Send the reply using NBDriverCommunicator
                nbd_driv_coomm.SendReply(request);

            }  //end else if 

        }//end for 

    }//end while

return 0;

//end try
} catch (const std::exception& e) {
    std::cerr << "Exception caught in main: " << e.what() << std::endl;
    return 1; 
}

catch(...)
{
    std::cerr << "Unknown exception occurred." << std::endl;
    return 1;
}

}// end main

