/*******************************************************************************
 ***** Author: Daniel Shabso
 ***** Reviewer:
 ***** Date: 18/09/2023
 ***** Version: 1
 ***** Description: Shuffle and Sort Dictinary File
 *******************************************************************************/
#define _POSIX_C_SOURCE 199309L /*Enable CLOCK_MONOTONIC*/

#include <assert.h> /*assert*/
#include <string.h> /*memcpy*/
#include <stdlib.h> /*calloc*/
#include <fcntl.h>  /*open*/
#include <stdio.h>  /*fprintf*/
#include <time.h>
#include <pthread.h>    /*pthread_create*/

#include <sys/types.h>  /*fstat*/
#include <sys/stat.h>   /*fstat*/
#include <sys/mman.h>   /*mmap*/

#include "../include/aux_funcs.h" /*RETURN_IF_ERROR*/

enum
{
    NUM_COPIES = 3,
    NUM_THREADS = 12
};


typedef struct
{
    char **array;
    int start;
    int end;
} thread_data_t;

/************************* Function prototypes  *****************************/
void Shuffle(char **array, int len_);
void Insertionsort(char** array, int start_, int end_);
void BubbleSort(char** array, int start_, int end_);
void *SortSegment(void *arg_);
void Merge(char **array, int start_, int mid_, int end_);
void MergeSort(char **array, int start_, int end_);

/********************************** Main  ***********************************/

int main(void)
{
    int status = 0;
    struct stat statbuf = {0};
    int i = 0, j = 0;
    int count_num_str = 0, size_bytes = 0, fd = 0;

    char **string_ptrs = NULL;
    char *dict = NULL;

    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data_t[NUM_THREADS];
    int segment_size = 0;
    int remainder = 0;
    int current_index = 0;

    struct timespec start, end;
    long elapsed_time;

    /*TIME: record the start time*/
    status = clock_gettime(CLOCK_MONOTONIC, &start);
    RETURN_IF_ERROR(-1 == status, "Failed clock_gettime start", 1);

    /*TIME: read dictionary into memory */
    fd = open("/usr/share/dict/american-english", O_RDONLY);
    RETURN_IF_ERROR(-1 == fd, "Failed to open dictionary file", 1);


    /*FIND NUM BYTES: number of bytes (characters) */
    status = fstat(fd, &statbuf);
    RETURN_IF_ERROR(-1 == status, "Failed fstat", 1);
    size_bytes = statbuf.st_size;

    /*MAP*/
    dict = mmap(NULL, size_bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    RETURN_IF_ERROR(MAP_FAILED == dict, "Failed mmap", 1);

    /*make each \n into \0 */
    for (i = 0; i < size_bytes; ++i)
    {
        if ('\n' == dict[i])
        {
            dict[i] = 0;
            ++count_num_str;
        }
    }
    /*preparing array of pointers to strings, will be used for sorting*/
    string_ptrs = calloc(count_num_str *NUM_COPIES, sizeof(char *));
    RETURN_IF_ERROR(NULL == string_ptrs, "Failed calloc", 1);
    string_ptrs[0] = dict;

    /*creates the first copy of the array of pointers*/
    for (i = 0; i < size_bytes; ++i)
    {
        if (0 == dict[i])
        {
            ++j;    /*j = current ciunting num of string in dict*/
            string_ptrs[j] = dict + i + 1;
        }
    }

    /*creates multiple copies of the array of pointers*/
    for (i = 1; i < NUM_COPIES; ++i)
    {
        memcpy(string_ptrs + (i *count_num_str), string_ptrs, sizeof(char*) *count_num_str);
    }

    /*shuffle the array of pointers*/
    Shuffle(string_ptrs, count_num_str *NUM_COPIES);

    /*create and launch threads*/
    segment_size = count_num_str *NUM_COPIES / NUM_THREADS;
    remainder = count_num_str *NUM_COPIES % NUM_THREADS;

    for (i = 0; i < NUM_THREADS; i++)
    {
        int segment_end = 0;
        thread_data_t[i].array = string_ptrs;
        thread_data_t[i].start = current_index;

        segment_end = current_index + segment_size;
        if (i < remainder)
        {
            segment_end++;
        }

        thread_data_t[i].end = segment_end;

        status = pthread_create(&threads[i], NULL, SortSegment, &thread_data_t[i]);
        RETURN_IF_ERROR(0 != status, "Failed to create thread", 1);

        current_index = segment_end;
    }

    MergeSort(string_ptrs, 0, count_num_str *NUM_COPIES - 1);

    /*wait for threads to complete*/
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    /*record the end time*/
    clock_gettime(CLOCK_MONOTONIC, &end);
    RETURN_IF_ERROR(-1 == status, "Failed clock_gettime end", 1);

     /*calculate the elapsed time in nanoseconds*/
    elapsed_time = (end.tv_sec - start.tv_sec) * 1000000000L
                                         + (end.tv_nsec - start.tv_nsec); 

    for (i = 0; i < count_num_str *NUM_COPIES; ++i)
    {
        printf("string_ptrs[%d] :>> %s\n", i ,string_ptrs[i]);
    }

    printf("Elapsed time: %ld nanoseconds\n", elapsed_time);
    printf("Elapsed time: %ld seconds\n", elapsed_time/1000000000);

    return 0;
}

/**************************** Functions Definition*******************************/
void Shuffle(char **array, int len_)
{
    int i = 0, j = 0;
    char *temp = NULL;

    assert(NULL != array);

    for (i = len_ - 1; i > 0; i--)
    {
        j = rand() % (i + 1);
        /*Swap pointers array[i] and array[j]*/
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }    
}

void Insertionsort(char** array, int start_, int end_) 
{
    int i = 0, j = 0;

    for (i = start_ + 1; i < end_; i++) 
    {
        char* key = array[i];
        j = i - 1;
        while (j >= start_ && strcmp(array[j], key) > 0) 
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }
}

void BubbleSort(char** array, int start_, int end_) 
{
    int i = 0, j = 0;

    for (i = start_; i < end_; i++)
    {
        for (j = start_; j < end_ - 1; j++)
        {
            if (strcmp(array[j], array[j + 1]) > 0)
            {
                /*Swap pointers array[j] and array[j + 1]*/
                char*temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void *SortSegment(void *arg_)
{
    thread_data_t *data = (thread_data_t *)arg_;
    char** array = data->array;
    int start = data->start;
    int end = data->end;

    assert(NULL != arg_);

    Insertionsort(array, start, end);   /*FAST*/
    /*BubbleSort(array, start, end);*/  /*SLOW*/

    pthread_exit(NULL);
}


/*Merge two sorted arrays into one sorted array */
void Merge(char **array, int start_, int mid_, int end_)
{
    int left_size = mid_ - start_ + 1;
    int right_size = end_ - mid_;

    /*Create temporary arrays for the left and right segments*/
    char **left = (char **)malloc(left_size *sizeof(char *));
    char **right = (char **)malloc(right_size *sizeof(char *));

    int i = 0, j = 0, k = 0;

    /*Copy data to temporary arrays left[] and right[]*/
    for (i = 0; i < left_size; i++)
    {
        left[i] = array[start_ + i];
    }

    for (j = 0; j < right_size; j++)
    {
        right[j] = array[mid_ + 1 + j];
    }

    /*Merge the two arrays back into array[start..end]*/
    i = 0, j = 0, k = start_;
    while (i < left_size && j < right_size)
    {
        if (strcmp(left[i], right[j]) <= 0)
        {
            array[k++] = left[i++];
        }
        else
        {
            array[k++] = right[j++];
        }
    }

    /*Copy the remaining elements of left[], if any*/
    while (i < left_size)
    {
        array[k++] = left[i++];
    }

    /*Copy the remaining elements of right[], if any*/
    while (j < right_size)
    {
        array[k++] = right[j++];
    }

    /*Free temporary arrays*/
    free(left);
    free(right);
}

/*Merge sort function for sorting and merging segments */
void MergeSort(char **array, int start_, int end_)
{
    int mid = 0;

    assert(NULL != array);

    if (start_ < end_)
    {
        mid = start_ + (end_- start_) / 2;

        /*Sort the first and second halves*/
        MergeSort(array, start_, mid);
        MergeSort(array, mid + 1, end_);

        /*Merge the sorted halves*/
        Merge(array, start_, mid, end_);
    }
}