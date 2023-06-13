/* diffrenece in C between char* arr[] and char** arr
ChatGPT
In C, char* arr[] and char** arr are both declarations involving pointers to pointers, 
but they have slightly different meanings.

char* arr[]:
	This declares arr as an array of pointers to characters. 
	It is often referred to as an array of strings, where each 
	element of the array is a pointer to a null-terminated character 
	array (string). Essentially, it represents a two-dimensional array 
	where each row is a string. 
	The size of the array must be specified at compile-time.
Example:
	char* arr[] = {"Hello", "World"};
	
char** arr:
	This declares arr as a pointer to a pointer to a character.
	It is commonly used to represent a pointer to a dynamically 
	allocated two-dimensional array or an array of pointers to characters. 
	The size of the array can be determined at runtime.
Example:
	char** arr = malloc(2 * sizeof(char*));
	arr[0] = "Hello";
	arr[1] = "World";
	
In summary, the main difference is that char* arr[] is an array of pointers to characters, 
often used to represent an array of strings, while char** arr is a pointer to a pointer to 
a character, typically used for dynamically allocated two-dimensional arrays or arrays of pointers.

*/

#include <stdio.h> /* printf, size_t */
#include <stdlib.h> /* malloc, free */


static void try1(char* arr[]) /* it is actually converts to char** in this case */
{
	printf("----------------------\n");
	printf("%p\n", &arr);
	printf("%p\n", &arr[0]);
	
	printf("%lu\n", sizeof(arr));
	printf("%s\n", arr[0]);
}
static void try2(char* arr[2])
{
	/* this has no problem, so in this case also, 
	   arr is char** ? 
	*/
	printf("%s\n", arr[3]);
}

static void try3()
{
	char* arr1[] = {"Hello", "World", "Bye"};
	printf("sizeof(arr1) is %lu    --> 24 (8 * 3) \n", sizeof(arr1));
	printf("num of elements is %lu --> 3          \n", sizeof(arr1) / sizeof(arr1[0]));
	
	printf("-------------------------------------------------------------------------------\n");
	
	char** arr2 = malloc(3 * sizeof(char*));
	arr2[0] = "Hello"; arr2[1] = "World"; arr2[2] = "Bye";
	
	printf("sizeof(arr2) is %lu     --> 8 (a pointer) \n", sizeof(arr1));
	printf("num of elements is %lu  --> 1 (unknown)   \n", sizeof(arr1) / sizeof(arr1[0]));
}


static void try4()
{
	{
	char strs[3][5] = { "12345", "abcd", "1234" };
	
	printf("%s\n", strs);    // will print '12345abcd' bcs the first has no \0
	printf("%s\n", strs[0]); // same
	printf("%s\n", strs[1]); // abcd
	}
	{
	char* strs[] = {"Hello", "World", "Bye"};
	// printf("%s\n", strs);    // not recommended... although might not crash
	printf("%s\n", strs[0]);    // Hello
	}
	{
	char** strs = malloc(3 * sizeof(char*));
	strs[0] = "Hello";  strs[1] = "World"; strs[2] = "Bye";
	free(strs);
	}
	
}



/* int main(int argc, char* argv[]) */
int main()
{
	char** arr = malloc(4 * sizeof(char*));
	arr[0] = "Hello";
	arr[1] = "World";
	arr[2] = "Bye";
	arr[3] = "now";
	
	printf("%p\n", &arr);
	printf("%p == %p\n", &arr[0], arr);
	
	if (0) {
		try1(arr);
		try2(arr);
		try3();
	}
	try4();
	/* (arr); */
	free(arr);
	return 0;
}

