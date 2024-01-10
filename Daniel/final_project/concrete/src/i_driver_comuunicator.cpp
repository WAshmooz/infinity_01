#include <iostream>
#include <memory>
#include <stdexcept>
#include "driver_data.hpp"

namespace hrd31 
{

class DriverError : public std::runtime_error 
{
public:
    explicit DriverError(const std::string& message) : 
                                                  std::runtime_error(message) {}
};

class IDriverCommunicator 
{
public:
    explicit IDriverCommunicator() = default;
    virtual ~IDriverCommunicator() noexcept = default;
    IDriverCommunicator(const IDriverCommunicator& other_) = delete;
    IDriverCommunicator& operator=(const IDriverCommunicator& other_) = delete;

    virtual std::shared_ptr<DriverData> ReceiveRequest() = 0;
    virtual void SendReply(std::shared_ptr<DriverData> data_) = 0;
    virtual void Disconnect() = 0;
    virtual int GetRequestFD() = 0;
};

} // namespace hrd31
