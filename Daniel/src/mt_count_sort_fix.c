
/*******************************************************************************
 *****  Author: Daniel Shabso
 *****  Reviewer:
 *****  Date: 12/09/2023
 *****  Version: 2
 *****  Description: Multi Threaded Countiong Sort
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h> 
#include <pthread.h>
#include <unistd.h>


#include <sys/mman.h>/*mmap*/
#include <sys/stat.h> /*struct stat*/
#include <fcntl.h> /*O_RDONLY*/

enum
{
    NUM_THREADS = 4,
    CASHLINE_SIZE = 64,
    ASCI_SIZE = 256
};

typedef struct
{
    pthread_mutex_t *mutex;
    char *data;
    size_t size;
    int *histogram_general_ptr; /*General histogram*/
} thread_args_t;

/******************************Global variables********************************/

/****************************Function prototypes*******************************/
void printCharArray(const int *buffer);
void *CountElement(void *args_);
void ProcessFile(const char *file_path, int *histogram_general);

/**********************************Functions***********************************/

void printCharArray(const int *buffer)
{
    size_t i = 0;
    for (i = 0; i < ASCI_SIZE; i++)
    {
        if (0 != buffer[i])
            printf("[Dec] %ld      [char] %c = %d [repetitions] \n",i, (char)i, buffer[i]);
    }
    printf("\n");
}

/*Thread function to count occurrences of an element*/
void *CountElement(void *args_)
{
    thread_args_t *counting = (thread_args_t *)args_;
    size_t i = 0;
    int *histogram_local = (int *)memalign(2 * CASHLINE_SIZE, ASCI_SIZE * sizeof(int)); /*TODO change to malloc*/

    /*initialize histograma to 0*/
    for (i = 0; i < ASCI_SIZE; i++)
    {
        histogram_local[i] = 0;
    }

    /*counting histograma*/
    for (i = 0; i < (counting->size); ++i)
    {
        histogram_local[(int)((counting->data)[i])] += 1;
    }

    /*lock mutex and update counting in the histogram*/
    pthread_mutex_lock(counting->mutex);
    for (i = 0; i < ASCI_SIZE; ++i)
    {
        counting->histogram_general_ptr[i] += histogram_local[i];
    }
    pthread_mutex_unlock(counting->mutex);

    free(histogram_local);
    histogram_local = NULL;
    pthread_exit(NULL);
}

void ProcessFile(const char *file_path, int *histogram_general)
{
    struct stat st;
    size_t data_size = 0;
    size_t chunk_size_common = 0;
    size_t chunk_size_end = 0;
    int i = 0, last_thread_index = 0;
    char *data = NULL;

    pthread_t threads[NUM_THREADS];
    thread_args_t thread_args[NUM_THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    /*Open the file for reading*/
    int fd = open(file_path, O_RDONLY);
    if (fd == -1)
    {
        perror("Failed to open file");
        exit(1);
    }

    /*Get the file size*/
    if (fstat(fd, &st) == -1)
    {
        perror("Failed to get file size");
        close(fd);
        exit(1);
    }

    data_size = st.st_size;

    /*Map the file into memory and close it*/
    data = (char *)mmap(NULL, data_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    if (data == MAP_FAILED)
    {
        perror("Mapping failed");
        exit(1);
    }

    /*Divide the work among threads*/
    chunk_size_common = (data_size / NUM_THREADS);
    chunk_size_end = (chunk_size_common + data_size % NUM_THREADS);

    for (i = 0; i < NUM_THREADS - 1; ++i)
    {
        thread_args[i].mutex = &mutex;
        thread_args[i].data = data + (i * chunk_size_common);
        thread_args[i].size = chunk_size_common;
        thread_args[i].histogram_general_ptr = histogram_general;

        pthread_create(&threads[i], NULL, CountElement, &thread_args[i]);
    }

    last_thread_index = NUM_THREADS - 1;
    thread_args[last_thread_index].mutex = &mutex;
    thread_args[last_thread_index].data = data + (last_thread_index * chunk_size_common);
    thread_args[last_thread_index].size = chunk_size_end;
    thread_args[last_thread_index].histogram_general_ptr = histogram_general;

    pthread_create(&threads[last_thread_index], NULL, CountElement, &thread_args[last_thread_index]);

    /*Wait for all threads to finish*/
    for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    /*Unmap the memory*/
    if (munmap(data, data_size) == -1)
    {
        perror("Unmapping failed");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    int histogram_general[ASCI_SIZE] = {0};
    const char *file_path = NULL;

    if (argc != 2)
    {
        printf("Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    file_path = argv[1];

    ProcessFile(file_path, histogram_general);

    printCharArray(histogram_general);

    return 0;
}












