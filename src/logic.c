#include "logic.h"

#include <stdlib.h>
#include <limits.h>

#include "array.h"
#include "moves.h"

bool loss = false;

bool checkLoss(Board* board) {
	return loss;
}

int evaluate(Board* board) {
	int score = 0;
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			if (pieceAt(board->orange, x, y)) {
				score -= 1;
			} else if (pieceAt(board->black, x, y)) {
				score += 1;
			}
		}
	}
	if (board->orangeToMove) {
		return score;
	}
	return -score;
}

int negaMax(Board* board, int alpha, int beta, int depth) {
	if (checkLoss(board)) {
		if (board->orangeToMove) {
			return INT_MAX;
		} else {
			return INT_MIN;
		}
	}
	if (depth == 0) {
		return evaluate(board);
	}
	Array moves;
	initArray(&moves, 32, sizeof(Move));
	getMoves(board, &moves);
	Board* b = malloc(sizeof(Board));
	for (int i = 0; i < moves.size; i++) {
		if (moves.array[i] == NULL) {
			continue;
		}
		copyBoard(b, board);
		Move* m = (Move*) moves.array[i];
		move(b, m->startX, m->startY, m->endX, m->endY);
		int score = -negaMax(b, -beta, -alpha, depth - 1);
		if (score >= beta) {
			return beta;
		} else if (score > alpha) {
			alpha = score;
		}
	}
	free(b);
	for (int i = 0; i < moves.size; i++) {
		free(moves.array[i]);
	}
	freeArray(&moves);
	return alpha;
}

void makeMove(Board* board) {
	// List possible moves
	Array possibleMoves;
	initArray(&possibleMoves, 32, sizeof(Move*));
	getMoves(board, &possibleMoves);

	// Evaluate moves
	Array bestMoves;
	initArray(&bestMoves, 4, sizeof(Move*));
	int max = INT_MIN;
	int moveIndex = 0;
	Board* b = malloc(sizeof(Board));
	bool noLegalMoves = true;
	for (int i = 0; i < possibleMoves.size; i++) {
		if (possibleMoves.array[i] == NULL) {
			continue;
		}
		noLegalMoves = false;
		copyBoard(b, board);
		Move* m = (Move*) possibleMoves.array[i];
		move(b, m->startX, m->startY, m->endX, m->endY);
		int n = negaMax(b, INT_MIN, INT_MAX, 2);
		if (n > max) {
			max = n;
			clearArray(&bestMoves);
			addElement(&bestMoves, m);
		} else if (n == max) {
			addElement(&bestMoves, m);
		}
	}
	free(b);

	if (noLegalMoves) {
		loss = true;
	} else {
		// Make move
		Move* m = (Move*) (bestMoves.array[randi() % bestMoves.size]);
		char* toMove = board->black;
		char* opponent = board->orange;
		if (board->orangeToMove) {
			toMove = board->orange;
			opponent = board->black;
		}
		if (pieceAt(toMove, m->startX, m->startY) == CHIEF && pieceAt(opponent, m->endX, m->endY) == CHIEF) {
			loss = true;
		}
		move(board, m->startX, m->startY, m->endX, m->endY);
	}
	
	// Free memory
	for (int i = 0; i < possibleMoves.size; i++) {
		free(possibleMoves.array[i]);
	}
	freeArray(&possibleMoves);
	freeArray(&bestMoves);
}
