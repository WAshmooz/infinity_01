
static void TestInsert(void)
{
	size_t i = 0;
	heap_t *heap_ptr = NULL;
	int arr[NUM_OF_TESTS] = {11, 10, 3, 4, 5, 6, 7, 8, 9, 2};
	int *data = NULL;
	
	heap_ptr = HeapCreate(IsBefore);
	
	for (i = 0; i < NUM_OF_TESTS; i++)
	{
		if (HeapPush(heap_ptr, &arr[i]) != SUCCESS)
		{
			printf("HeapPush failed for i = %ld\n", i);
		}
		
		data = HeapPeek(heap_ptr);
		printf("HeapPeek = %d, ", *data);
		printf("HeapSize = %ld\n", HeapSize(heap_ptr));
		
		if ( (i+1) != HeapSize(heap_ptr) ) 
		{
			printf("HeapPush failed HeapSize for i = %ld\n", i);	
		}
		PrintHeap(heap_ptr);
	}
	
	PrintHeap(heap_ptr);
	
	HeapDestroy(heap_ptr);
	heap_ptr = NULL;
}

