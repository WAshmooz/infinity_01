#include <stdio.h>
#include <stdlib.h>




void PrintTree(void *vec, const size_t SIZE)
{
	int i = 0, j = 0, level = 0;
	int curIdx = 0;
	int nodes_on_level = 1;
	int curNode;
	int safeStop = 100;
	int treeSpaces1[] = { 
		126, 0,
		62,  126,
		30, 62,
		14, 30,
		6, 14,
		2,  6,
		0,  2 };
	int treeSpaces2[] = { 
		62,  0,
		30, 62,
		14, 30,
		6, 14,
		2,  6,
		0,  2 };
	
	int *treeSpaces = treeSpaces2;
	int spacesBefore, spacesInBetween;
	
	while (--safeStop && curIdx < SIZE)
	{
		spacesBefore = treeSpaces[level++];
		spacesInBetween = treeSpaces[level++]; 
		printf("%*s", spacesBefore, "");
		
		for (i = 0; i < nodes_on_level && curIdx < SIZE; ++i, ++curIdx)
		{
			curNode = *(int*)VectorGetAccess(vec, curIdx);
			printf("%02d", curNode);
			
			if (i % 2) {
				printf("%*s", spacesInBetween, "");
			}
			else {
				for (j = 0; j < spacesInBetween; ++j) printf("-");
				/* printf("%-s", half-4, ""); */
			}
		}
		nodes_on_level *= 2;
		printf("\n");
	}
	/* printf("\n\n--------------------------------------------------------------\n"); */
}



