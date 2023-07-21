#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   /* ssize_t, size_t */

struct cbuffer
{
	size_t capacity;
	char *read;
	char *write;
	char arr[1];
};

typedef struct cbuffer cbuffer_t;

cbuffer_t *CBufferCreate(size_t capacity)
{
	size_t buffsize = 0;
	buffsize = offsetof(cbuffer_t, arr) + (capacity + 1)*sizeof(char); 
	
	cbuffer_t *cbuffer = (cbuffer_t *)malloc(buffsize);
	
	if(NULL == cbuffer)
	{
		return NULL;
	}
	
	cbuffer->capacity = capacity;
	cbuffer->read = cbuffer->arr;
	cbuffer->write = cbuffer->arr;
	
	return cbuffer;
}

#define SIZE 1000

void try1(char* buff)
{
	cbuffer_t cb;
	int i;
	
	printf("%p\n", &cb.arr[0]);
	printf("%p\n", &cb.arr[1]);
	printf("%p\n", &cb.arr[2]);
	printf("%p\n", &cb.arr[3]);
	
	
	for (i = 0; i < SIZE - 4; ++i) {
		cb.arr[i] = 'a';
	}
	cb.arr[SIZE - 4] = '\0';
	printf("%s\n", cb.arr);
	
}
void try2(const char* buff)
{
	cbuffer_t *pcb = CBufferCreate(SIZE);
	int i;
	for (i = 0; i < SIZE - 4; ++i) {
		pcb->arr[i] = 'a';
	}
	pcb->arr[SIZE - 4] = '\0';
	printf("%s\n", pcb->arr);
	
}




int main()
{
	char buff[SIZE];
	int i = 0;
	for (i = 0; i < SIZE; ++i) {
		buff[i] = (char)(i);
	}
	
	try1(buff);
	try2(buff);
	printf("----------------------\n");
	
	
	
	
	/* printf("%d\n", sizeof(cbuffer_t)); */
	/* printf("%d\n", sizeof(*cb)); */
	
	/* printf("%p\n", &cb->read); */
	/* printf("%p\n", &cb->write); */
	/* printf("%p\n", &cb->arr); */
	/* printf("%d\n", sizeof(cb->arr)); */
	
	return 0;
}

