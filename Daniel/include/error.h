#ifndef __ERROR_H__
#define __ERROR_H__

/*******************************************************************************
description:   sets output to red color
return value:  none
*******************************************************************************/
void ERROR(char* file, int line)
{
	printf("\033[1;31m");
	printf("FAILD IN FILE : %s AND LINE %d \n", file, line);
	printf("\033[0m");
}

#endif
