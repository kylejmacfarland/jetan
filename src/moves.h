#ifndef MOVES_H
#define MOVES_H

#include "array.h"
#include "board.h"

typedef struct {
	int startX;
	int startY;
	int endX;
	int endY;
} Move;

void getMoves(Board* board, Array* moves);

void move(Board* board, int startX, int startY, int endX, int endY);

#endif
