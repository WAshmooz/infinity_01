#include <stdio.h>  /* printf */
#include <stdlib.h> /* */
#include <string.h> /* strcpy */

#define SIZE 4

enum element_type
{
		INT = 0,
		STRING = 1,
		FLOAT = 2
};

typedef struct Element
{
	enum element_type my_type;  /* tell us to which type my void* is pointing to */
	void* my_ptr;               /* can point to int* or char* or float* */
	
} Element;


void AddElement(Element* pelem, int add)
{
	if (NULL == pelem->my_ptr)
	{
	 return;
	}
	
	char* tmp = NULL;
	char str_add[8];
	
	switch (pelem->my_type)
	{
	case INT: 
		*(int*)(pelem->my_ptr) += add;
		break;
	case STRING: 
		sprintf(str_add, "%d", add);
		tmp = (char*)pelem->my_ptr;  /* will be free after copy */

		pelem->my_ptr = malloc(8 + strlen(tmp)); /* for now more 8 bytes is enough */
		strcpy((char*)pelem->my_ptr, tmp);
		strcat((char*)pelem->my_ptr, str_add);
		free(tmp);
		break;
	case FLOAT: 
		*(float*)(pelem->my_ptr) += (float)add;
		break;
	}
}



void PrintElement(Element elem)
{
	if (NULL == elem.my_ptr)
	{
	 return;
	}
	switch (elem.my_type)
	{
	case INT: 
		printf("print int: %d\n", *((int*)(elem.my_ptr)));
		break;
		
	case STRING: 
		printf("print string: %s\n", (char*)elem.my_ptr);
		break;
		
	case FLOAT: 
		printf("print float: %f\n", *((float*)elem.my_ptr));
		break;		
	}
}


int main(int argc, char* argv[])
{
	Element arr[SIZE];
	int i = 0;
	int add_main = 10;
	
	/* initialize void* pointers to NULL */
	for (i = 0; i < SIZE; ++i)
	{
		arr[i].my_ptr = NULL;
	}
	
	/* create first element of type int: */
	arr[0].my_type = INT;
	arr[0].my_ptr = malloc(sizeof(int));
	
	*((int*)(arr[0].my_ptr)) = 88;
	
	
	/* create second element of type string */
	arr[1].my_type = STRING;
	arr[1].my_ptr = malloc(8 * sizeof(char));
	strcpy((char*)arr[1].my_ptr, "hello");
	
	/* create therd element of type float */
	arr[2].my_type = FLOAT;
	arr[2].my_ptr = malloc(sizeof(float));
	*((float*)arr[2].my_ptr) = 12.2;
	
	
	
	/* print elements */
	for (i = 0; i < SIZE; ++i)
	{
		PrintElement(arr[i]);
	}
	/* add elements */
	for (i = 0; i < SIZE; ++i)
	{
		AddElement(&arr[i], add_main);
	}
	/* print elements after add */
	for (i = 0; i < SIZE; ++i)
	{
		PrintElement(arr[i]);
	}
	
	
	/* free void* pointers */
	for (i = 0; i < SIZE; ++i)
	{
		if (NULL != arr[i].my_ptr) {
			free(arr[i].my_ptr);
			arr[i].my_ptr = NULL;
		}
	}
	
	
	return 0;
}
