/* -----------------------------------------------------------------------------
Description: Waitable Thread-safe priority queue that allows threads to wait 
             until an element becomes available if the queue is empty
Author: HRD31
Reviewer: Itzik
Version:
    v0.2 - Mentor approved
----------------------------------------------------------------------------- */
#ifndef __HRD31_WAITABLE_PQ_HPP__
#define __HRD31_WAITABLE_PQ_HPP__

#include <queue>
#include <mutex>
#include <condition_variable>


namespace hrd31
{

// element type/ container type for the pqueue/ Compare function (for sorting)
template <typename T,
        typename Container = std::vector<T>,
        typename Compare = std::less<typename Container::value_type>>
class WPQueue
{
public:
    explicit WPQueue( const Compare& compare = Compare(),
                        const Container& cont = Container() );
    ~WPQueue() = default;
    WPQueue(const WPQueue& other_) = delete;
    WPQueue& operator=(const WPQueue& other_) = delete;

    /*  Pop(): Removes and returns the top element of the priority queue. 
        If the queue is empty, it waits until an element becomes available in
        the queue using a condition variable (m_cv).
        We change it to return by value and not by referece (problems
        with the shrink of vector and invalid refernce due to it)*/
    T Pop();

    /*  Inserts the specified element into the pqueue and notifies 
        one waiting thread if any*/
    void Push(const T& element_);

    size_t Size() const;
    bool IsEmpty() const;

private:
    std::priority_queue<T, Container, Compare> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_cv; //used for block and waking threads based on queue state changes
};

// mutex - to synchronize access to the shared resource (pqueue)
// Cond Var - to block and wake threads based on changes in the queue's state.

/******************************************************************************/

//Ctor - Initializes the pqueue with the provided compare function and container 
template <typename T, typename Container, typename Compare>
WPQueue<T, Container, Compare>::WPQueue(const Compare& compare, 
                                        const Container& cont)
                                                     : m_queue(compare, cont) {}


//Pop - Removes and returns the top element of the pqueue
template <typename T, typename Container, typename Compare>
T WPQueue<T, Container, Compare>::Pop()
{
    std::unique_lock<std::mutex> lock(m_mutex);

/* Uses the condition variable (m_cv) to blocks the current thread until the 
   condition specified in the lambda function becomes true (queue is't empty)
   if tru - wait() function will unblock the thread and allow it to proceed.*/

    m_cv.wait(lock, [this] { return !m_queue.empty(); });


/* option 1: Returns the copied element by value - copying  the top element 
            of the queue. In our case it doesnt mater becase our T is shared ptr 
            and it uses counting (like RC) without making a copy*/

    T top = m_queue.top(); // this line use copy operation (but not in out case)
    m_queue.pop();
    return top;


/* option 2: Returns the moved element as an rvalue reference - ownership 
            transfer without a copy

    T top = std::move(m_queue.top()); //improving performance: avoid costly copy 
    m_queue.pop();
    return std::move(top); // Return the moved element as an rvalue reference
*/
}



//Push - Inserts element into the pqueue and notifies one waiting thread if any.
template <typename T, typename Container, typename Compare>
void WPQueue<T, Container, Compare>::Push(const T& element_)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(element_);
    }// Lock released when goes out of scope

    m_cv.notify_one(); //unblocks one of the waiting threads.
}

//Size()
template <typename T, typename Container, typename Compare>
size_t WPQueue<T, Container, Compare>::Size() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.size();
}// Lock released when goes out of scope

//IsEmpty() 
template <typename T, typename Container, typename Compare>
bool WPQueue<T, Container, Compare>::IsEmpty() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.empty();
}// Lock released when goes out of scope


}//namespace hrd31

#endif //__HRD31_WAITABLE_PQ_HPP__
