/*
Description: Test for the Singleton (is it really make only one instance)
Author: Daniel
Version: 
    v1 - implementation of TestSingleton() and test1() to check if it creates
         only one instance as it should 
    v2 - add test2() to test explicit creation of instance from shared obj
    v3 - add test3() to test implicit creation of instance from shared obj
Note: do not run test1() and test2() at the same runtime
*/

#include <iostream>
#include <thread>
#include <vector>
#include <dlfcn.h>

#include "singleton.hpp"

// manages a single instance of the TestClass
void TestSingleton() 
{
    TestClass* my_singleton_test = Singleton<TestClass>::GetInstance();
    my_singleton_test->SomeFunction(); //relevant only for test1()
    std::cout << "Address : " << my_singleton_test << std::endl;
}


int test1() 
{
    std::vector<std::thread> threads;

    // Create 5 threads to test Singleton to verify only one instance created
    for (int i = 0; i < 5; ++i) 
    {
        threads.emplace_back(TestSingleton);
    }

    // Wait for all threads to finish
    for (auto iter = threads.begin(); iter != threads.end(); ++iter) 
    {
        auto& element = *iter;
        element.join();
    }

    std::cout << "\n";

    return 0;
}


int test2() 
{
    std::cout << "--------- START TEST 2 (explicit) -------------" << std::endl;
    
    // Load the shared object dynamically
    void* handle = dlopen("./libshared.so", RTLD_LAZY);
    if (!handle) 
    {
        std::cerr << "Cannot load shared object: " << dlerror() << std::endl;
        return 1;
    }

    // Get the function pointer for foo from the shared object 
    void (*foo)() = reinterpret_cast<void (*)()>(dlsym(handle, "foo"));
    if (!foo) 
    {
        std::cerr << "Cannot find function 'foo' in the shared object: " 
                                                      << dlerror() << std::endl;
        dlclose(handle);
        return 1;
    }

    // Call the function foo from the shared object
    foo();

    // Close the shared object
    dlclose(handle);

    // Create another instance in main and print its address
    TestClass* instance = Singleton<TestClass>::GetInstance();
    std::cout << "Address from main: " << instance << std::endl;

    return 0;
}

/******************************************************************************/
extern "C" 
{
    void foo(); // Declare foo() function signature
}

int test3() 
{
    std::cout << "\n";

    // Explicitly call the foo() function
    foo();

    std::cout << "---------- START TEST 3 (implicit) ------------" << std::endl;

    // Create an instance in the main function
    TestClass* main_instance = Singleton<TestClass>::GetInstance();
    std::cout << "instance from main: " << main_instance << std::endl;

    return 0;
}

/******************************************************************************/

int main()
{
    test1();

    //test2();

    test3();

    return 0;
}




//FLAGS and cmd lines
/*
g++ -std=c++11 -I include -c -fPIC src/shared_singleton.cpp 
g++ -std=c++11 -I include test/singleton_test.cpp -o singleton_test -L. -Wl,-rpath,. -lshared
./singleton_test

*/