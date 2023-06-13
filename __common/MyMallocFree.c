#include "MyMallocFree.h"

struct MemoryLeak
{
	void* ptr;
	size_t numOfElements;
	size_t elementSize;
	int mallocTime;
	int freeTime;
};

#define MEMORY_LEAK_SIZE 128
MemoryLeak memleak[MEMORY_LEAK_SIZE] = { 0 };


static int FindNextFreeIndex()
{
	int i = 0;
	for (; i < MEMORY_LEAK_SIZE; ++i) {
		if (NULL == memleak[i].ptr) return i;
	}
	
	printf("ERROR: you have to increase memory array size\n");
	/* all have been used */
	return -1;
}

static int FindPtrInMemoryLeakArr(const void* ptr)
{
	int i = 0;
	for (; i < MEMORY_LEAK_SIZE; ++i) {
		if (ptr == memleak[i].ptr && 0 == memleak[i].freeTime) return i;
	}
	
	printf("ERROR: didn't find the pointer in MemoryLeak array\n");
	/* all have been used */
	return -1;
}

void* MyMalloc(const size_t numOfElements, const size_t elementSize)
{
	static int creation_counter = 0;
	int idx = FindNextFreeIndex();
	void* ptr = malloc(numOfElements * elementSize);
	
	memleak[idx].ptr = ptr;
	memleak[idx].numOfElements = numOfElements;
	memleak[idx].elementSize = elementSize;
	memleak[idx].mallocTime = ++creation_counter;
	
	return ptr;
}
void MyFree(void* ptr)
{
	static int free_counter = 0;
	int idx = FindPtrInMemoryLeakArr(ptr);
	free(ptr);
	
	if (-1 == idx) return;
	
	memleak[idx].freeTime = ++free_counter;
}

void MemoryLeak_Check()
{
	int i = 0;
	printf("\n");
	printf("idx |      ptr    | malloc | free | elements | elemSize \n");
	printf("----|-------------|--------|------|----------|--------- \n");
	
	for (; i < MEMORY_LEAK_SIZE; ++i) {
		if (NULL == memleak[i].ptr) return;
		
		
		printf(" %2d | %p |   %2d   |  %2d  |    %2lu    |   %2lu   \n",
				i, memleak[i].ptr, memleak[i].mallocTime, memleak[i].freeTime,
				memleak[i].numOfElements, memleak[i].elementSize);
		
	}
}



