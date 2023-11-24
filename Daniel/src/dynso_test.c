#include <stdio.h>
#include <dlfcn.h>

extern void Foo();
extern void Bla();

int main(int argc, char *argv[]) 
{
    if (argc != 3) 
    {
        printf("Usage: %s <shared_object> <function_name>\n", argv[0]);
        return 1;
    }

    const char *shared_object_path = argv[1];
    const char *function_name = argv[2];

    /*Load the shared object*/
    void *handle = dlopen(shared_object_path, RTLD_LAZY);
    if (!handle) 
    {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    /*Find the function using dlsym*/
    void (*function)() = dlsym(handle, function_name);
    if (!function) 
    {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    /*Execute the function*/
    (*function)();

    /*Close the shared object*/
    dlclose(handle);

    return 0;
}