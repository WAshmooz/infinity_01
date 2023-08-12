#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct Point
{
	int x;
	int y;
} Point;

typedef struct ChessBoard
{
	Point point[8][8];
	int isFree[8][8];
	
} ChessBoard;

static Point jumps[8] = {
	{-1, 2}, {-1,-2},  {-2,  1}, {2,  1}, 
	{ 1, 2}, { 1, -2}, {-2, -1}, {2, -1} };

int isLegalJump(Point *cur, Point *jump)
{
	int x = cur->x + jump->x;
	int y = cur->y + jump->y;
	if (x < 0 || 7 < x) return 0;
	if (y < 0 || 7 < y) return 0;
	return 1;
}
int isFreeJump(ChessBoard *board, Point *cur, Point *jump)
{
	int x = cur->x + jump->x;
	int y = cur->y + jump->y;
	if (x < 0 || 7 < x) return 0;
	if (y < 0 || 7 < y) return 0;
	
	if (board->isFree[x][y]) {
		board->isFree[x][y] = 0;
		cur->x = x;
		cur->y = y;
		return 1;
	}
	
	return 0;
}


int FindNextJump(ChessBoard *board, Point *cur)
{
	int i;
	for (i = 0; i < 8; ++i) {
		if (isFreeJump(board, cur, &jumps[i])) {
			return 1;
		}
	}
	
	/* no free option.. */
	return 0;
}

void JumpRandomPrev(Point *cur)
{
	int i;
	int legal[8] = { 0 };
	for (i = 0; i < 8; ++i) {
		if (isLegalJump(cur, &jumps[i])) {
			legal[i] = 1;
		}
	}
	
	i = rand() % 8;
	while (0 == legal[i]) {
		i = rand() % 8;
	}
	cur->x += jumps[i].x;
	cur->y += jumps[i].y;
}

void JumpRandom(Point *cur)
{
	int i;
	i = rand() % 8;
	while (!isLegalJump(cur, &jumps[i])) {
		i = rand() % 8;
	}
	cur->x += jumps[i].x;
	cur->y += jumps[i].y;
	
}



void FindNext(Point *cur)
{
	int i;
	for (i = 0; i < 8; ++i) {
		if (isLegalJump(cur, &jumps[i])) {
			cur->x += jumps[i].x;
			cur->y += jumps[i].y;
		}
	}
}



void InitChessBoard(ChessBoard *board)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < 8; ++i) {
		for (j = 0; j < 8; ++j) {
			board->point[i][j].x = i;
			board->point[i][j].y = j;
			board->isFree[i][j] = 1;
		}
	}
}

void PrintChessBoard(ChessBoard *board, Point *cur)
{
	int x, y;
	for (y = 7; 0 <= y; --y) {
		for (x = 0; x < 8; ++x) {
			if (x == cur->x && y == cur->y) printf("$ ");
			else printf("%d ", board->isFree[x][y]);
		}
		printf("\n");
	}
	printf("\n----------------------------\n");
}

/* 100,000,000 */

void RunHorse(ChessBoard *board)
{
	Point cur = {0, 0};
	int freePoints = 64;
	size_t iterations = 0;
	board->isFree[cur.x][cur.y] = 0;
	--freePoints;
	PrintChessBoard(board, &cur);
	while (freePoints && iterations < 1010) {
		if (0 == FindNextJump(board, &cur))  {
			JumpRandom(&cur);
		}
		else --freePoints;
		if (1000 <= iterations)
			PrintChessBoard(board, &cur);
		++iterations;
	}
	PrintChessBoard(board, &cur);
	
	printf("it took %zu iterations\n", iterations);
	
}



int main(int argc, char* argv[])
{
	ChessBoard board;
	InitChessBoard(&board);
	srand(time(NULL));
	RunHorse(&board);
	
	
	return 0;
}

