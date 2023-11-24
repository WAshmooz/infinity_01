/*******************************************************************************
 ***** Author: Daniel Shabso
 ***** Reviewer:
 ***** Date: 18/09/2023
 ***** Version: 1
 ***** Description: Shuffle and Sort Dictinary File
 *******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

enum
{
    NUM_COPIES = 3,
    NUM_THREADS = 4
};

#define RETURN_IF_ERROR(condition, msg, err_type) \
do { \
    if (((condition))) { \
        fprintf(stderr, "Error: %s (Line %d)\n", (msg), __LINE__); \
        return (err_type); \
    } \
} while (0)

#define UNUSED(x) (void)(x)

/*Thread data structure for sorting a portion of the array*/
typedef struct
{
    char **array;
    int start;
    int end;
} ThreadData;

/************************* Function prototypes  *****************************/
void Shuffle(char **array, int len_);
void *SortSegment(void *arg_);
void Merge(char **array, int start_, int mid_, int end_);
void MergeSort(char **array, int start_, int end_);

/********************************** Main  ***********************************/
void Shuffle(char **array, int len_)
{
    int i = 0, j = 0;
    char *temp = NULL;
    for (i = len_ - 1; i > 0; i--)
    {
        j = rand() % (i + 1);
        /*Swap pointers array[i] and array[j]*/
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }    
}

void *SortSegment(void *arg_)
{
    ThreadData*data = (ThreadData*)arg_;
    char**array = data->array;
    int start = data->start;
    int end = data->end;
    int i = 0, j = 0;

    /*Sort the segment using a sorting algorithm (e.g., quicksort)*/
    /*You can implement your sorting logic here*/

    /*For demonstration purposes, let's use a simple bubble sort*/
    for (i = start; i < end; i++)
    {
        for (j = start; j < end - 1; j++)
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

    pthread_exit(NULL);
}


int main(void)
{
    struct stat statbuf = {0};
    int i = 0, j = 0;
    int count_num_str = 0, size_bytes = 0, fd = 0;

    char **string_ptrs = NULL;
    char *dict = NULL;

    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    int segment_size = 0;
    int remainder = 0;
    int current_index = 0;

    /*Seed the random number generator */
    srand(time(NULL));

    /*read dict into memory */
    fd = open("/usr/share/dict/american-english", O_RDONLY);
    RETURN_IF_ERROR(-1 == fd, "Failed to open dictionary file", 1);


    /*number of bytes (characters) */
    fstat(fd, &statbuf);
    size_bytes = statbuf.st_size;

    dict = mmap(NULL, size_bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    /*make each \n into \0 */
    for (i = 0; i < size_bytes; ++i)
    {
        if ('\n' == dict[i])
        {
            dict[i] = 0;
            ++count_num_str;
        }
    }

    string_ptrs = calloc(count_num_str *NUM_COPIES, sizeof(char *));
    string_ptrs[0] = dict;

    for (i = 0; i < size_bytes; ++i)
    {
        if (0 == dict[i])
        {
            ++j;
            string_ptrs[j] = dict + i + 1;
        }
    }

    for (i = 1; i < NUM_COPIES; ++i)
    {
        memcpy(string_ptrs + (i *count_num_str), string_ptrs, sizeof(char*) *count_num_str);
    }

    /*Shuffle the array of pointers*/
    Shuffle(string_ptrs, count_num_str *NUM_COPIES);

    /*Create and launch threads*/
    segment_size = count_num_str *NUM_COPIES / NUM_THREADS;
    remainder = count_num_str *NUM_COPIES % NUM_THREADS;

    for (i = 0; i < NUM_THREADS; i++)
    {
        int segmentEnd = 0;
        threadData[i].array = string_ptrs;
        threadData[i].start = current_index;

        segmentEnd = current_index + segment_size;
        if (i < remainder)
        {
            segmentEnd++;
        }

        threadData[i].end = segmentEnd;

        pthread_create(&threads[i], NULL, SortSegment, &threadData[i]);

        current_index = segmentEnd;
    }

    MergeSort(string_ptrs, 0, count_num_str *NUM_COPIES - 1);

    /*Wait for threads to complete*/
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (i = 0; i < count_num_str *NUM_COPIES; ++i)
    {
        printf("string_ptrs[%d] :>> %s\n", i ,string_ptrs[i]);
    }

    return 0;
}



/*Merge two sorted arrays into one sorted array */
void Merge(char **array, int start_, int mid_, int end_)
{
    int leftSize = mid_ - start_ + 1;
    int rightSize = end_ - mid_;

    /*Create temporary arrays for the left and right segments*/
    char **left = (char **)malloc(leftSize *sizeof(char *));
    char **right = (char **)malloc(rightSize *sizeof(char *));

    int i = 0, j = 0, k = 0;

    /*Copy data to temporary arrays left[] and right[]*/
    for (i = 0; i < leftSize; i++)
    {
        left[i] = array[start_ + i];
    }
    for (j = 0; j < rightSize; j++)
    {
        right[j] = array[mid_ + 1 + j];
    }

    /*Merge the two arrays back into array[start..end]*/
    i = 0, j = 0, k = start_;
    while (i < leftSize && j < rightSize)
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
    while (i < leftSize)
    {
        array[k++] = left[i++];
    }

    /*Copy the remaining elements of right[], if any*/
    while (j < rightSize)
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