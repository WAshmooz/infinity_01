/*
Description: Singleton 
             A templated class that ensures only one instance of the specified 
             type T exists throughout the program.
Author: HRD31
Reviewer: Itzik
Version:
    v0.1 - For mentor approval
    v0.2 - heder aproved
    v1 - heder ready
    v1.1 - add TestClass to the header to create obj file ()
*/
//#include <iostream>
#include <memory>
#include <mutex>
#include <atomic>

/*******    Instructions for the class that uses Singleton *****/
// Must define default Ctor
// Default Ctor must be declared in private
// Using class must declare Singleton as a friend


template <typename T>
class Singleton
{
public:
    /*  A static method responsible for providing access to the single instance 
        of the class. It returns a pointer to the instance of type T*/
    static T* GetInstance(); //lifetime of obj till the process ends 

    Singleton() = delete;
    ~Singleton() = delete;
    Singleton(const Singleton<T>& other_) = delete;
    Singleton<T>& operator=(const Singleton<T>& other_) = delete;

private:
    static std::atomic<T*> s_instance; // 1
    static std::mutex s_mutex; // 2
};



template<typename T>
std::mutex Singleton<T>::s_mutex; 

template<typename T>
std::atomic<T*> Singleton<T>::s_instance(nullptr);


template<typename T>
T* Singleton<T>::GetInstance()
{
    T* tmp = s_instance.load(std::memory_order_relaxed); 

    std::atomic_thread_fence(std::memory_order_acquire); //4
    if (!tmp) //if instance has not yet been created.
    {
        //2
        std::lock_guard<std::mutex> lock(s_mutex); //to prevent multiple threads from simultaneously creating an instance.
        tmp = s_instance.load(std::memory_order_relaxed);
        if (!tmp) //Re-checks tmp within the locked section to ensure only one instance is created.
        {
            tmp = new T;
            static std::unique_ptr<T> destoyer(tmp); // release the tmp.
            //we use static to enlarge the lifetime of obj till the process ends

            std::atomic_thread_fence(std::memory_order_release); //3
            s_instance.store(tmp, std::memory_order_relaxed);
        }
    }

    return s_instance;
}


//comments 

/*  1. . A static atomic pointer responsible for holding the single instance of 
        the class. It ensures atomic operations for thread safety.*/

/*  2. .A static mutex used for thread safety during instance creation to prevent 
        multiple threads from creating instances simultaneously.*/

/*  3. establishes that the changes made to s_instance are visible 
    to other threads after its initialization. */

/*  4. ensures that any changes made by other threads up to this point are also 
    visible to the current thread.*/


/*  the second memory fence helps maintain proper memory visibility and 
    synchronization across threads, ensuring that the changes made by other 
    threads are correctly observed by the current thread after the 
    critical section.*/



/* this class should't be here is here becase i didint provided him header and 
   i needed it for the shared obj i created */

/*   
class TestClass 
{
public:
    ~TestClass() { std::cout << "TestClass destroyed" << std::endl; }

    //relevant only for test1()
    void SomeFunction() { std::cout << "Do something..." << std::endl; }

private:
    // Make the default constructor private
    TestClass() {std::cout << "TestClass created\n" << std::endl;}

    // Allow Singleton class access to private constructor
    friend class Singleton<TestClass>;
};
*/
