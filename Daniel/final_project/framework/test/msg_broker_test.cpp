#include <iostream>

#include "msg_broker.hpp"
#include "msg_broker_imp.hpp"

int test1() 
{
    // Create a my_dispatcher and an Observer
    hrd31::Dispatcher<hrd31::Message> my_dispatcher;
    hrd31::Observer my_observer;

    // Create a my_callback and register it with the dispatcher
    hrd31::CallBack<hrd31::Message, hrd31::Observer> my_callback(my_dispatcher, 
            													 my_observer, 
            								   &hrd31::Observer::OnReceive, 
            								   &hrd31::Observer::OnStop);

    // Notifying subscribers
    hrd31::Message msg1(42);
    hrd31::Message msg2(99);

    my_dispatcher.NotifyAll(msg1);
    my_dispatcher.NotifyAll(msg2);

    return 0;
}



int main()
{
    test1();

    return 0;
}
