#include <iostream>
#include <thread>
#include <vector>

#include "waitable_pq.hpp"

//function simulates a producer by pushing integer values into the WPQueue
void Producer(hrd31::WPQueue<int>& pq, int start, int end) 
{
    for (int i = start; i <= end; ++i) 
    {
        pq.Push(i);
        std::cout << "<--Poshed: " << i << std::endl;

        // Simulating work for 100 milliseconds
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }
}

//function simulates a consumer by popping values from the WPQueue
void Consumer(hrd31::WPQueue<int>& pq, int count) 
{
    for (int i = 0; i < count; ++i) 
    {
        int value = pq.Pop();

        //print what poped
        std::cout << "      -->Popped: " << value << std::endl;
    }
}

int main() 
{
    //Creates an instance of WPQueue type int
    hrd31::WPQueue<int> priorityQueue;

    // Creating producer and consumer threads
    std::thread producer(Producer, std::ref(priorityQueue), 1, 10);
    std::thread consumer(Consumer, std::ref(priorityQueue), 5);

    // Joining threads
    producer.join();
    consumer.join();

    return 0;
}
