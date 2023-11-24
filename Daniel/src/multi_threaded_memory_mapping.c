/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			25.08.23 
****Last update:  		25.08.23
****Version:			1
****Description:		Prooducers-Consumers  

**************************	Header & Macro	********************************/
#include <stdio.h>   /* printf */
#include <stdlib.h>   /* malloc */
#include <pthread.h> /* pthread_spin_init, pthread_spin_lock, pthread_spin_unlock */
#include <unistd.h>

/***************************	Ex2     ************************************/
int global_variable = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

__thread int a;

/*Function executed by threads*/
void *ThreadFunction(void *arg) 
{
    int local_variable = 0;
    void *thread_stack_addr = NULL;
    void *heap_addr = NULL;
    
    /*Determine the stack address*/
    thread_stack_addr = &local_variable;

    /*Allocate memory on the heap*/
    heap_addr = malloc(sizeof(int));

    /*Print memory locations*/
    printf("Thread ID:  %ld\n", pthread_self());
    printf("Thread Stack Address:           %p, %ld\n", thread_stack_addr, pthread_self());
    /*printf("Thread Local Storage Address: %p\n", &local_variable);*/
    printf("Global Variable Address:        %p, %ld\n", &global_variable, pthread_self());
    printf("Heap Address:                   %p, %ld\n", heap_addr, pthread_self());
    printf("\n-----------------------------------------\n");

    pthread_mutex_lock(&mutex); 
    ++global_variable;
    pthread_mutex_unlock(&mutex);
    printf("Global counter:                   %d, %ld\n", &global_variable, pthread_self());
    printf("__thread a:                   %d, %ld\n", &a, pthread_self());


    free(heap_addr); /*Free the allocated heap memory*/

    return NULL;
}

int main() 
{
    int i = 0;
    pthread_t threads[5];

    /*Create multiple threads*/
    for (i = 0; i < 5; i++) 
    {
        pthread_create(&threads[i], NULL, ThreadFunction, NULL);
    }

    /*Wait for threads to finish*/
    for (i = 0; i < 5; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}



Thread ID:  140336886179392
Thread Stack Address:           0x7fa2ba3fde34, 140336886179392
Global Variable Address:        0x55c608b18040, 140336886179392
Heap Address:                   0x7fa2ac000b70, 140336886179392
Thread ID:  140336894572096
Thread Stack Address:           0x7fa2babfee34, 140336894572096
Global Variable Address:        0x55c608b18040, 140336894572096
Heap Address:                   0x7fa2b4000b70, 140336894572096

-----------------------------------------
Global counter:                   145850432, 140336894572096

-----------------------------------------
Global counter:                   145850432, 140336886179392
__thread a:                   -1170217412, 140336886179392
__thread a:                   -1161824708, 140336894572096
Thread ID:  140336743568960
Thread Stack Address:           0x7fa2b1bfce34, 140336743568960
Global Variable Address:        0x55c608b18040, 140336743568960
Heap Address:                   0x7fa2a4000b70, 140336743568960

-----------------------------------------
Global counter:                   145850432, 140336743568960
Thread ID:  140336869393984
Thread ID:  140336877786688
Thread Stack Address:           0x7fa2b9bfce34, 140336877786688
Global Variable Address:        0x55c608b18040, 140336877786688
Heap Address:                   0x7fa2b4000b70, 140336877786688

-----------------------------------------
Global counter:                   145850432, 140336877786688
__thread a:                   -1178610116, 140336877786688
__thread a:                   -1312827844, 140336743568960
Thread Stack Address:           0x7fa2b93fbe34, 140336869393984
Global Variable Address:        0x55c608b18040, 140336869393984
Heap Address:                   0x7fa2ac000b70, 140336869393984

-----------------------------------------
Global counter:                   145850432, 140336869393984
__thread a:                   -1187002820, 140336869393984





process 154423                  tls = 0x7ffff7bff63c    main = 0x5555555553f4
Mapped address spaces:

          Start Addr           End Addr       Size     Offset  Perms  objfile                                                                                                                                
      0x555555554000     0x555555555000     0x1000        0x0  r--p   /home/daniel/git/system_programming/src/a.out
      0x555555555000     0x555555556000     0x1000     0x1000  r-xp   /home/daniel/git/system_programming/src/a.out
      0x555555556000     0x555555557000     0x1000     0x2000  r--p   /home/daniel/git/system_programming/src/a.out
      0x555555557000     0x555555558000     0x1000     0x2000  r--p   /home/daniel/git/system_programming/src/a.out
      0x555555558000     0x555555559000     0x1000     0x3000  rw-p   /home/daniel/git/system_programming/src/a.out
      0x555555559000     0x55555557a000    0x21000        0x0  rw-p   [heap]
      0x7ffff73ff000     0x7ffff7400000     0x1000        0x0  ---p
      0x7ffff7400000     0x7ffff7c00000   0x800000        0x0  rw-p
      0x7ffff7c00000     0x7ffff7c28000    0x28000        0x0  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7c28000     0x7ffff7dbd000   0x195000    0x28000  r-xp   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7dbd000     0x7ffff7e15000    0x58000   0x1bd000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7e15000     0x7ffff7e19000     0x4000   0x214000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7e19000     0x7ffff7e1b000     0x2000   0x218000  rw-p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7e1b000     0x7ffff7e28000     0xd000        0x0  rw-p
      0x7ffff7fa4000     0x7ffff7fa7000     0x3000        0x0  rw-p
      0x7ffff7fbb000     0x7ffff7fbd000     0x2000        0x0  rw-p
      0x7ffff7fbd000     0x7ffff7fc1000     0x4000        0x0  r--p   [vvar]
      0x7ffff7fc1000     0x7ffff7fc3000     0x2000        0x0  r-xp   [vdso]
      0x7ffff7fc3000     0x7ffff7fc5000     0x2000        0x0  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7fc5000     0x7ffff7fef000    0x2a000     0x2000  r-xp   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7fef000     0x7ffff7ffa000     0xb000    0x2c000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffb000     0x7ffff7ffd000     0x2000    0x37000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffd000     0x7ffff7fff000     0x2000    0x39000  rw-p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffffffde000     0x7ffffffff000    0x21000        0x0  rw-p   [stack]
  0xffffffffff600000 0xffffffffff601000     0x1000        0x0  --xp   [vsyscall]
