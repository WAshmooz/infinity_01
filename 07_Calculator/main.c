#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef long ssize_t;


const char *GREEN = "\033[0;32m";
const char *RED = "\033[1;31m";
const char *NC = "\033[0m";



typedef enum EType
{
	IS_OPERAND = 0,
	IS_OPERATOR = 1,
	IS_NONE = 2,
	
} EType;

typedef struct Operators_and_Numbers
{
	double num;
	char op;
	EType e_type;
	
} Operators_and_Numbers;

Operators_and_Numbers arr[64];

/*
 *str is the pointer to the string,
 ch is pointer to char which will hold the next operator
 return -1 if there was problem and no operator found
 */
static int getOperator(char **str, char *ch)
{
	char res = ' ';
	while (**str == ' ') ++(*str);
	
	res = **str;
	if ('(' == res || '+' == res || '*' == res || '^' == res || 
	    ')' == res || '-' == res || '/' == res )
	{
		++(*str);
		*ch = res;
		return 0;
	}
	
	if ('\n' == res) {
		printf("We reached end of line - OK\n");
	}
	else {
		printf("ERROR char: [%c] - check input", res);
	}
	
	return -1; /* ERROR */
}

static int getFromString(char* str)
{
	double ret;
	char ch;
	char *ptr = str;
	int i = 0;
	int safeStop = 100;
	
	while (*ptr && --safeStop) {
		ret = strtod(str, &ptr);
		
		if (0.0 == ret && str == ptr) {
			/* printf("not number\n"); */
			if (-1 == getOperator(&ptr, &ch))
			{
				break;
			}
			
			arr[i].e_type = IS_OPERATOR;
			arr[i].op = ch;
		}
		else {
			arr[i].e_type = IS_OPERAND;
			arr[i].num = ret;
		}
		++i;
		str = ptr;
	}
	arr[i].e_type = IS_NONE;
	
	printf("%sHere is the Operands and Operators: %s\n", GREEN, NC);
	for (i = 0; arr[i].e_type != IS_NONE; ++i) {
		if (arr[i].e_type == IS_OPERAND) {
			printf("    Num: %lf\n", arr[i].num);
		}
		else  {
			printf("    Op:  %c\n", arr[i].op);
		}
	}
	printf("\n");
	
	
	return 0;
}


static int readFromFileAndDo()
{
	char* line = NULL;
	size_t len = 0;
	ssize_t readLen;
	
	FILE* file = fopen("input.txt", "r");
	if (file == NULL) {
		perror("Failed to open the file");
		return EXIT_FAILURE;
	}
	
	/* Read lines using the C version of getline */
	while ((readLen = getline(&line, &len, file)) != -1) {
		/* Process the line */
		printf("%p] %zu lines: %s", line, readLen, line);
		
		if (0 == strncmp("END", line, 3)) break;
		getFromString(line);
	}
	
	fclose(file);
	
	/* Free the allocated memory */
	if (line) free(line);
	
	return EXIT_SUCCESS;
}


int main ()
{
	return readFromFileAndDo();
	return 0;;
}



