/* -----------------------------------------------------------------------------
Description: Message broker (Publisher-subscriber)
Author: HRD31
Reviewer: Itzik
Version:
    v0.1 - For mentor approval
----------------------------------------------------------------------------- */
#ifndef __HRD31_MSG_BROKER_HPP__
#define __HRD31_MSG_BROKER_HPP__

#include <vector>
#include <iostream> 

//#include "msg_broker_imp.hpp"

namespace hrd31
{
// Forward declaration of ICallBack
template<typename MSG>
class ICallBack;

/**********************   Dispatcher   **********************/
template<typename MSG>
class Dispatcher
{
public:
    explicit Dispatcher() = default;
    ~Dispatcher();
    Dispatcher(const Dispatcher& other_) = delete;
    Dispatcher& operator=(const Dispatcher& other_) = delete;

    void NotifyAll(const MSG& msg_);

private:
    std::vector<ICallBack<MSG>*> m_subscribers;
    void Register(ICallBack<MSG>* call_back_);
    void Unregister(ICallBack<MSG>* call_back_);
    
    friend class ICallBack<MSG>;
};

/**********************   ICallBack   **********************/
template<typename MSG>
class ICallBack
{
public:
    explicit ICallBack(Dispatcher<MSG>* dispatcher_);
    virtual ~ICallBack();

    ICallBack(const ICallBack& other_) = delete;
    ICallBack& operator=(const ICallBack& other_) = delete;

private:
    Dispatcher<MSG>* const m_dispatcher;

    virtual void Notify(const MSG& msg_) = 0;
    virtual void Disconnect() = 0;

    friend class Dispatcher<MSG>;
};

/**********************   CallBack   **********************/
template<typename MSG, typename Observer>
class CallBack : public ICallBack<MSG>
{
public:
    using DoAction = void(Observer::*)(const MSG&);
    using DoStop = void(Observer::*)();

    explicit CallBack(Dispatcher<MSG>& dispatcher_, Observer& observer_,
        DoAction action_func_, DoStop stop_func_ = nullptr);

    virtual ~CallBack();
    CallBack(const CallBack& other_) = delete;
    CallBack& operator=(const CallBack& other_) = delete;

    void Notify(const MSG& msg_) override;
    void Disconnect() override;

private:
    Observer& m_observer;
    DoAction m_action_func;
    DoStop m_stop_func;
};

}//namespace hrd31

#endif //__HRD31_MSG_BROKER_HPP__