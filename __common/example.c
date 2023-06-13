
#include "MyMallocFree.h"

int main()
{
	int* intArr = (int*)MyMalloc(4, sizeof(int));
	char* charArr = (char*)MyMalloc(2, sizeof(char));
	
	/* ↑ the above is a wrapper for malloc, instead of this,  */
	if (0) { 
		intArr = (int*)malloc(4 * sizeof(int));
		charArr = (char*)malloc(2 * sizeof(char));
	}
	/* int* intArr = (int*)malloc(sizeof(node_t)); */
	
	intArr[0] = 1; intArr[1] = 22; intArr[2] = 333; intArr[3] = 4444;
	charArr[0] = 'a';
	charArr[1] = 'b';
	
	
	MemoryLeak_Check();
	MyFree(intArr);
	MemoryLeak_Check();
	MyFree(charArr);
	MemoryLeak_Check();
	
	
	return 0;
}

