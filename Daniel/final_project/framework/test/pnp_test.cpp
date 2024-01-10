#include <iostream>
#include <dlfcn.h>
#include <thread> //sleep

#include "pnp.hpp"

int main() 
{
    hrd31::PlugNPlay plug_n_play("./pnp");
    std::this_thread::sleep_for(std::chrono::seconds(100));

    std::cout << "Test completed successfully!" << std::endl;

    return 0;
}

/*
COMPILATION FOR SHARED OBJECT (.os):
g++ -std=c++11  -I include -shared -fPIC pnp_lib.cpp src/i_task.cpp -o libsample.so

COMPILATION FOR EXECUTE (.exe):
g++ -std=c++11 -I include -I./pnp src/pnp.cpp test/pnp_test.cpp -lpthread -rdynamic -g -L./pnp -Wl,-rpath,./pnp
*/

