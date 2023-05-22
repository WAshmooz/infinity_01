#include <stdio.h>  /* printf */
#include <stdlib.h> /* */
#include <string.h> /* strcpy */


#define SIZE 4

enum element_type {
		INT,
		STRING,
		FLOAT
};

typedef struct Element
{
	enum element_type m_type;  /* tell us to which type my void* is pointing to */
	void* m_ptr;               /* can point to int* or char* or float* */
	
} Element;



void PrintElement(Element elem)
{
	if (NULL == elem.m_ptr) return;
	
	switch (elem.m_type)
	{
	case INT: 
		printf("print int: %d\n", *((int*)elem.m_ptr));
		break;
	case STRING: 
		printf("print string: %s\n", (char*)elem.m_ptr);
		break;
	}
}



int main(int argc, char* argv[])
{
	Element arr[SIZE];
	int i;
	
	/* initialize void* pointers to NULL */
	for (i = 0; i < SIZE; ++i)
	{
		arr[i].m_ptr = NULL;
	}
	
	/* create first element of type int: */
	arr[0].m_type = INT;
	arr[0].m_ptr = malloc(sizeof(int));
	
	/* create second element of type string */
	arr[1].m_type = STRING;
	arr[1].m_ptr = malloc(8 * sizeof(char));
	strcpy((char*)arr[1].m_ptr, "hello");
	
	
	/* print elements */
	for (i = 0; i < SIZE; ++i)
	{
		PrintElement(arr[i]);
	}
	
	
	free(arr[0].m_ptr);
	free(arr[1].m_ptr);
	
	
	
	return 0;
}


