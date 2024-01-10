/* -----------------------------------------------------------------------------
Description: Request engine task 
Author: HRD31
Reviewer: Itzik
Version:
    v0.1 - For mentor approval
    v0.2 - Mentor approved
----------------------------------------------------------------------------- */
#ifndef __HRD31_ARGS_NBD_FW_HPP__
#define __HRD31_ARGS_NBD_FW_HPP__

#include <cstring> //strcmp
#include <cstdio> //fileno(stdin)

#include "driver_data.hpp"
#include "nbd_driver_communicator.hpp"
#include "ram_storage.hpp"
#include "re_fw.hpp"


#define FILE_DESCRIPTOR_STDIN fileno(stdin)

namespace hrd31
{

struct IArgs //intrface 
{
    virtual ~IArgs() = default;
}; 

/*------------------------------------------------------------------------------
                        Args InputSrc
------------------------------------------------------------------------------*/

struct NBDArgs : public IArgs 
{
public:
    
    NBDArgs(hrd31::NBDriverCommunicator& nbd_comm_,
            hrd31::RAMStorage& ram_storage_) :
          m_nbd_comm(nbd_comm_),
          m_ram_storage(ram_storage_) {}

    ~NBDArgs() = default;
    hrd31::NBDriverCommunicator& m_nbd_comm;
    hrd31::RAMStorage& m_ram_storage;
    std::shared_ptr<hrd31::DriverData> m_driv_data; 
};


class InputSrcNBD : public hrd31::IInputSrc<req_type_t, std::shared_ptr<IArgs>> 
{
public:
    
    explicit InputSrcNBD(hrd31::NBDriverCommunicator& nbd_comm_,
                         hrd31::RAMStorage& ram_storage_) :
                         m_nbd_args(std::make_shared<NBDArgs>(nbd_comm_, ram_storage_))
    {
    }

    ~InputSrcNBD() = default;
    int GetFd() override 
    {
        return m_nbd_args->m_nbd_comm.GetRequestFD();
    }

    std::pair<req_type_t, std::shared_ptr<IArgs>> Read() override 
    {
        // Receive the request from the NBD driver
        m_nbd_args->m_driv_data  = m_nbd_args->m_nbd_comm.ReceiveRequest();

        // Prepare the data to be sent to the Request Engine
        return std::make_pair(m_nbd_args->m_driv_data->m_type, m_nbd_args);
    }

private:
    std::shared_ptr<NBDArgs> m_nbd_args;
};


class NBDTask : public IRETask 
{
public:
    explicit NBDTask(std::shared_ptr<NBDArgs> args_)
        : m_nbd_args(args_) {}

    ~NBDTask() = default;
/*
    static std::shared_ptr<NBDTask> Create(std::shared_ptr<IArgs> args_)
    {
        return std::make_shared<NBDTask>(std::dynamic_pointer_cast<NBDArgs>(args_));
    }
*/
    static std::shared_ptr<NBDTask> Create(std::shared_ptr<IArgs> args_)
    {
        auto derived_args = std::dynamic_pointer_cast<NBDArgs>(args_);
        if (!derived_args) 
        {
            std::cout << "NBDTask -> static Create faild" << std::endl;
            return nullptr; //throw execption
        }
        return std::make_shared<NBDTask>(derived_args);
    }

private:
    void Execute() override 
    {
        std::shared_ptr<hrd31::DriverData> request = m_nbd_args->m_driv_data;
        // Process the received request based on its type
        switch (request->m_type) 
        {
            case hrd31::READ: 
            {
                std::cout << "Executing read" << std::endl;
               m_nbd_args->m_ram_storage.Read(request);
                break;
            }
            case hrd31::WRITE: 
            {
                std::cout << "Executing write" << std::endl;
                m_nbd_args->m_ram_storage.Write(request);
                break;
            }
            default:
                //other request types..
                break;
        }

        // Send the reply using NBDriverCommunicator
         m_nbd_args->m_nbd_comm.SendReply(request);
    }

    std::shared_ptr<NBDArgs> m_nbd_args;
};

/*------------------------------------------------------------------------------
                                    Stdin
------------------------------------------------------------------------------*/

struct StdinArgs : public IArgs 
{
public:
    StdinArgs(hrd31::RequestEngine<req_type_t, std::shared_ptr<IArgs>>& req_eng_,
              hrd31::NBDriverCommunicator& nbd_comm_)
        : m_req_eng(req_eng_), m_nbd_comm(nbd_comm_), m_buff_input{0}{}

    ~StdinArgs() = default;

    hrd31::RequestEngine<req_type_t, std::shared_ptr<IArgs>>& m_req_eng;
    hrd31::NBDriverCommunicator& m_nbd_comm;
    char m_buff_input[2];
};


class InputSrcStdin : public hrd31::IInputSrc<req_type_t, std::shared_ptr<IArgs>> 
{
public:
    explicit InputSrcStdin(hrd31::RequestEngine<req_type_t, std::shared_ptr<IArgs>>& req_eng_,
                           hrd31::NBDriverCommunicator& nbd_comm_)
        : m_stdin_args(std::make_shared<StdinArgs>(req_eng_, nbd_comm_))
    {
    }

    ~InputSrcStdin() = default;
    
    int GetFd() override 
    {
        return FILE_DESCRIPTOR_STDIN;
    }

    std::pair<req_type_t, std::shared_ptr<IArgs>> Read() override 
    {
        read(STDIN_FILENO, m_stdin_args->m_buff_input, 1);
        return std::make_pair(STOP, m_stdin_args);
    }

private:
    std::shared_ptr<StdinArgs> m_stdin_args;
    
};


class StinTask : public IRETask 
{
public:
    explicit StinTask(std::shared_ptr<StdinArgs> args_)
        : m_stdin_args(args_) {}

    ~StinTask() = default;

    static std::shared_ptr<StinTask> Create(std::shared_ptr<IArgs> args_)
    {
        auto derived_args = std::dynamic_pointer_cast<StdinArgs>(args_);
        if (!derived_args) 
        {
            //std::cout << "StinTask -> static Create faild" << std::endl;
            return nullptr; //throw execption
        }
        return std::make_shared<StinTask>(derived_args);
        //std::cout << "StinTask -> static Create Done" << std::endl;
    }

private:
    void Execute() override 
    {
        //std::cout << "StinTask -> Execute(): " << std::string(m_stdin_args->m_buff_input) << std::endl;
        if (!strncmp(m_stdin_args->m_buff_input, "q", 1)) 
        {
            m_stdin_args->m_req_eng.Stop();
            m_stdin_args->m_nbd_comm.Disconnect();
        }
    }

    std::shared_ptr<StdinArgs> m_stdin_args;
};



}//namespace hrd31
#endif //__HRD31_ARGS_NBD_FW_HPP__


