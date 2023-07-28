#include <stdio.h> /* printf, size_t */
#include <stdlib.h> /* malloc, free */ 

typedef struct MemoryLeak MemoryLeak;

void* MyMalloc(const size_t numOfElements, const size_t elementSize);
void MyFree(void* ptr);
void MemoryLeak_Check();

