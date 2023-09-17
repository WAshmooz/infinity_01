#include "aux_funcs.h"

#include <stdio.h> /*fprintf, scanf*/
#include <stdlib.h> /*exit */


void ExitIfError(int condition, const char *msg, int err_type)
{
	if (!condition)
	{
        fprintf(stderr, "Error: %s (Line %d)\n", msg, __LINE__); 
		exit(err_type);
	}
}

