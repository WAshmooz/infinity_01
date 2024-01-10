/* -----------------------------------------------------------------------------
Description: Message broker - Template implementations 
Author: HRD31
Reviewer: Itzik
Version:
    v0.1 - For mentor approval
----------------------------------------------------------------------------- */
#ifndef __HRD31_MSG_BROKER_IMP_HPP__
#define __HRD31_MSG_BROKER_IMP_HPP__

#include<assert.h>
#include <algorithm> //std::find, for Unregister()

#include "msg_broker.hpp"

namespace hrd31
{
/**********************   Define the Template types ***************************/
// Define Message type
struct Message
{
    int data;
    Message(int d) : data(d) {}
};


// Define Observer class
class Observer 
{
public:
    void OnReceive(const Message& Message) 
    {
        std::cout << "Received Message with data: " << Message.data << std::endl;
    }

    void OnStop() 
    {
        std::cout << "Observer stopped" << std::endl;
    }
};

/*****************************   Dispatcher   *********************************/
template<typename MSG>
Dispatcher<MSG>::~Dispatcher()
{
    //Go over the vector and Disconnect all 
    for (auto& subscriber : m_subscribers)
    {
        subscriber->Disconnect();
    }
}

template<typename MSG>
void Dispatcher<MSG>::NotifyAll(const MSG& msg_)
{
    //Go over the vector and Notify all 
    for (auto& subscriber : m_subscribers)
    {
        subscriber->Notify(msg_);
    }
}

template<typename MSG>
void Dispatcher<MSG>::Register(ICallBack<MSG>* call_back_)
{
    //Push the call_back to the vector
    m_subscribers.push_back(call_back_);
}

template<typename MSG>
void Dispatcher<MSG>::Unregister(ICallBack<MSG>* call_back_)
{   
    // Find the call_back in the vector and erase it 
    auto it = std::find(m_subscribers.begin(), m_subscribers.end(), call_back_);
    if (it != m_subscribers.end())
    {
        m_subscribers.erase(it);
    }
}

/*
template<typename MSG>
std::vector<ICallBack<MSG>*> Dispatcher<MSG>::GetRegister() const
{
    return m_subscribers;
}
std::vector<ICallBack<hrd31::Message>*> subscribers = dispatcher.GetRegister();
*/
/*****************************   ICallBack   **********************************/
template<typename MSG>
ICallBack<MSG>::ICallBack(Dispatcher<MSG>* dispatcher_) : m_dispatcher(dispatcher_)
{
    m_dispatcher->Register(this); 
}

template<typename MSG>
ICallBack<MSG>::~ICallBack()
{
    m_dispatcher->Unregister(this);
}

/******************************   CallBack   **********************************/
template<typename MSG, typename Observer>
CallBack<MSG, Observer>::CallBack(Dispatcher<MSG>& dispatcher_, 
                                  Observer& observer_,
                                  DoAction action_func_, 
                                  DoStop stop_func_)

                                : ICallBack<MSG>(&dispatcher_), 
                                  m_observer(observer_), 
                                  m_action_func(action_func_), 
                                  m_stop_func(stop_func_)
{
}

template<typename MSG, typename Observer>
CallBack<MSG, Observer>::~CallBack()
{
    Disconnect();
}

template<typename MSG, typename Observer>
void CallBack<MSG, Observer>::Notify(const MSG& msg_)
{
    //Call the action function of the observer 
    (m_observer.*m_action_func)(msg_);
}

template<typename MSG, typename Observer>
void CallBack<MSG, Observer>::Disconnect()
{
    if(m_stop_func)
    {
        (m_observer.*m_stop_func)();
    }
}
}//namespace hrd31

#endif //__HRD31_MSG_BROKER_IMP_HPP__
