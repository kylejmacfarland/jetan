#include "moves.h"

#include <stdlib.h>

bool isSpaceThreatened(Board* board, bool checkOrangeThreat, int spaceX, int spaceY) {
	char* sideA = board->black;
	char* sideB = board->orange;
	if (checkOrangeThreat) {
		sideA = board->orange;
		sideB = board->black;
	}
	Array moves;
	initArray(&moves, 32, sizeof(Move*));
	int startIndex = 0;
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			switch (pieceAt(sideA, x, y)) {
			case PANTHAN:
				addPanthanMoves(&moves, sideA, checkOrangeThreat, x, y);
				break;
			case THOAT:
				addThoatMoves(&moves, sideA, x, y);
				break;
			case WARRIOR:
				addOrthogonalMoves(&moves, sideA, sideB, x, y, 2);
				addDiagonalMoves(&moves, sideA, sideB, x, y, 2);
				break;
			case PADWAR:
				addDiagonalMoves(&moves, sideA, sideB, x, y, 2);
				break;
			case DWAR:
				addDiagonalMoves(&moves, sideA, sideB, x, y, 3);
				break;
			case FLIER:
				addFlierMoves(&moves, sideA, x, y);
				break;
			case CHIEF:
				addOrthogonalMoves(&moves, sideA, sideB, x, y, 3);
				break;
			default:
				continue;
			}
			for (int i = startIndex; i < moves.size; i++) {
				if (((Move*) moves.array[i])->endX == spaceX && ((Move*) moves.array[i])->endY == spaceY) {
					for (int j = 0; j < moves.size; j++) {
						free(moves.array[j]);
					}
					freeArray(&moves);
					return true;
				}
			}
			startIndex = moves.size;
		}
	}
	for (int i = 0; i < moves.size; i++) {
		free(moves.array[i]);
	}
	freeArray(&moves);
	return false;
}

bool isPrincessInCheck(Board* board, bool checkOrange) {
	char* side = board->black;
	if (checkOrange) {
		side = board->orange;
	}
	int princessX = 0;
	int princessY = 0;
	for (int i = 0; i < 100; i++) {
		if (pieceAt(side, i % 10, i / 10) == PRINCESS) {
			princessX = i % 10;
			princessY = i / 10;
			break;
		}
	}
	if (isSpaceThreatened(board, !checkOrange, princessX, princessY)) {
		return true;
	}
	return false;
}

void addPanthanMoves(Array* moves, char* toMove, bool checkOrange, int x, int y) {
	for (int yOffset = -1; yOffset <= 1; yOffset++) {
		for (int xOffset = -1; xOffset <= 1; xOffset++) {
			if (xOffset == 0 && yOffset == 0) {
				continue;
			}
			if (checkOrange) {
				if (xOffset == 0 && yOffset == -1) {
					continue;
				}
			} else {
				if (xOffset == 0 && yOffset == 1) {
					continue;
				}
			}
			if (pieceAt(toMove, x + xOffset, y + yOffset) == EMPTY) {
				Move* m = malloc(sizeof(Move));
				m->startX = x;
				m->startY = y;
				m->endX = x + xOffset;
				m->endY = y + yOffset;
				addElement(moves, m);
			}
		}
	}
}

void addThoatMoves(Array* moves, char* toMove, int x, int y) {
	int offset[] = {
		 2,  1, 
		-2,  1, 
		 2, -1, 
		-2, -1, 
		 1,  2, 
		 1, -2, 
		-1,  2, 
		-1, -2
	};
	for (int i = 0; i < 8; i++) {
		if (pieceAt(toMove, x + offset[i * 2], y + offset[i * 2 + 1]) == EMPTY) {
			Move* m = malloc(sizeof(Move));
			m->startX = x;
			m->startY = y;
			m->endX = x + offset[i * 2];
			m->endY = y + offset[i * 2 + 1];
			addElement(moves, m);
		}
	}
}

void addWarriorMoves(Array* moves, char* toMove, char* opponent, int x, int y) {
	int offset[] = {
		 1,  0, 
		-1,  0, 
		 0,  1, 
		 0, -1, 
		 1,  1, 
		 1, -1, 
		-1,  1, 
		-1, -1, 
	};
	int xOffset = 0;
	int yOffset = 0;
	for (int i = 0; i < 8; i++) {
		xOffset = offset[i * 2];
		yOffset = offset[i * 2 + 1];
		if (pieceAt(toMove, x + xOffset, y + yOffset) != EMPTY) {
			continue;
		}
		Move* m = malloc(sizeof(Move));
		m->startX = x;
		m->startY = y;
		m->endX = x + offset[i * 2];
		m->endY = y + offset[i * 2 + 1];
		addElement(moves, m);
		if (pieceAt(opponent, xOffset, y + yOffset) != EMPTY) {
			continue;
		}
		xOffset += offset[i * 2];
		yOffset += offset[i * 2 + 1];
		if (pieceAt(toMove, x + xOffset, y + yOffset) != EMPTY) {
			continue;
		}
		m = malloc(sizeof(Move));
		m->startX = x;
		m->startY = y;
		m->endX = x + offset[i * 2];
		m->endY = y + offset[i * 2 + 1];
		addElement(moves, m);
	}
}

void addPadwarMoves(Array* moves, char* toMove, char* opponent, int x, int y) {
	int offset[] = {
		 1,  1, 
		-1,  1, 
		 1,  1, 
		-1, -1, 
	};
	int xOffset = 0;
	int yOffset = 0;
	for (int i = 0; i < 4; i++) {
		xOffset = offset[i * 2];
		yOffset = offset[i * 2 + 1];
		if (pieceAt(toMove, x + xOffset, y + yOffset) != EMPTY) {
			continue;
		}
		Move* m = malloc(sizeof(Move));
		m->startX = x;
		m->startY = y;
		m->endX = x + offset[i * 2];
		m->endY = y + offset[i * 2 + 1];
		addElement(moves, m);
		if (pieceAt(opponent, xOffset, y + yOffset) != EMPTY) {
			continue;
		}
		xOffset += offset[i * 2];
		yOffset += offset[i * 2 + 1];
		if (pieceAt(toMove, x + xOffset, y + yOffset) != EMPTY) {
			continue;
		}
		m = malloc(sizeof(Move));
		m->startX = x;
		m->startY = y;
		m->endX = x + offset[i * 2];
		m->endY = y + offset[i * 2 + 1];
		addElement(moves, m);
	}
}

void addOrthogonalMoves(Array* moves, char* toMove, char* opponent, int x, int y, int depth) {
	int offset[] = {
		 1,  0, 
		-1,  0, 
		 0,  1, 
		 0, -1
	};
	int xOffset = 0;
	int yOffset = 0;
	for (int i = 0; i < 4; i++) {
		xOffset = 0;
		yOffset = 0;
		for (int j = 0; j < depth; j++) {
			xOffset += offset[i * 2];
			yOffset += offset[i * 2 + 1];
			if (pieceAt(toMove, x + xOffset, y + yOffset) != EMPTY) {
				break;
			}
			Move* m = malloc(sizeof(Move));
			m->startX = x;
			m->startY = y;
			m->endX = x + xOffset;
			m->endY = y + yOffset;
			addElement(moves, m);
			if (pieceAt(opponent, xOffset, y + yOffset) != EMPTY) {
				break;
			}
		}
	}
}

void addDiagonalMoves(Array* moves, char* toMove, char* opponent, int x, int y, int depth) {
	int offset[] = {
		 1,  1, 
		-1,  1, 
		-1, -1, 
		 1, -1
	};
	int xOffset = 0;
	int yOffset = 0;
	for (int i = 0; i < 4; i++) {
		xOffset = 0;
		yOffset = 0;
		for (int j = 0; j < depth; j++) {
			xOffset += offset[i * 2];
			yOffset += offset[i * 2 + 1];
			if (pieceAt(toMove, x + xOffset, y + yOffset) != EMPTY) {
				break;
			}
			Move* m = malloc(sizeof(Move));
			m->startX = x;
			m->startY = y;
			m->endX = x + xOffset;
			m->endY = y + yOffset;
			addElement(moves, m);
			if (pieceAt(opponent, xOffset, y + yOffset) != EMPTY) {
				break;
			}
		}
	}
}

void addFlierMoves(Array* moves, char* toMove, int x, int y) {
	int offset[] = {
		 1,  1, 
		 2,  2,
		 3,  3,
		 1, -1, 
		 2, -2,
		 3, -3,
		-1,  1, 
		-2,  2,
		-3,  3,
		-1, -1, 
		-2, -2,
		-3, -3
	};
	for (int i = 0; i < 12; i++) {
		if (pieceAt(toMove, x + offset[i * 2], y + offset[i * 2 + 1]) == EMPTY) {
			Move* m = malloc(sizeof(Move));
			m->startX = x;
			m->startY = y;
			m->endX = x + offset[i * 2];
			m->endY = y + offset[i * 2 + 1];
			addElement(moves, m);
		}
	}
}

void addPrincessMoves(Array* moves, Board* board, int x, int y) {
	int offset[] = {
		 1,  0, 
		-1,  0, 
		 0,  1, 
		 0, -1
	};
	int xOffset = 0;
	int yOffset = 0;
	int depth = 3;
	if ((board->orangeToMove && !board->orangeEscaped) || (!board->orangeToMove && !board->blackEscaped)) {
		depth = 10;
	}
	for (int i = 0; i < 4; i++) {
		xOffset = 0;
		yOffset = 0;
		for (int j = 0; j < depth; j++) {
			xOffset += offset[i * 2];
			yOffset += offset[i * 2 + 1];
			if (pieceAt(board->black, x + xOffset, y + yOffset) == EMPTY && pieceAt(board->orange, x + xOffset, y + yOffset) == EMPTY) {
				Move* m = malloc(sizeof(Move));
				m->startX = x;
				m->startY = y;
				m->endX = x + xOffset;
				m->endY = y + yOffset;
				addElement(moves, m);
			}
		}
	}
}

void getMoves(Board* board, Array* moves) {
	char* toMove = board->black;
	char* opponent = board->orange;
	if (board->orangeToMove) {
		toMove = board->orange;
		opponent = board->black;
	}
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			switch (pieceAt(toMove, x, y)) {
			case PANTHAN:
				addPanthanMoves(moves, toMove, board->orangeToMove, x, y);
				break;
			case THOAT:
				addThoatMoves(moves, toMove, x, y);
				break;
			case WARRIOR:
				addOrthogonalMoves(moves, toMove, opponent, x, y, 2);
				addDiagonalMoves(moves, toMove, opponent, x, y, 2);
				break;
			case PADWAR:
				addDiagonalMoves(moves, toMove, opponent, x, y, 2);
				break;
			case DWAR:
				addDiagonalMoves(moves, toMove, opponent, x, y, 3);
				break;
			case FLIER:
				addFlierMoves(moves, toMove, x, y);
				break;
			case PRINCESS:
				addPrincessMoves(moves, board, x, y);
				break;
			case CHIEF:
				addOrthogonalMoves(moves, toMove, opponent, x, y, 3);
				break;
			}
		}
	}
	Board b;
	for (int i = 0; i < moves->size; i++) {
		copyBoard(&b, board);
		Move* m = (Move*) moves->array[i];
		move(&b, m->startX, m->startY, m->endX, m->endY);
		if (isPrincessInCheck(&b, board->orangeToMove)) {
			free(moves->array[i]);
			moves->array[i] = NULL;
		}
	}
}

void move(Board* board, int startX, int startY, int endX, int endY) {
	char* toMove = board->black;
	char* opponent = board->orange;
	if (board->orangeToMove) {
		toMove = board->orange;
		opponent = board->black;
	}
	if (pieceAt(toMove, startX, startY) == PRINCESS && (startX - endX > 3 || startY - endY > 3)) {
		if (board->orangeToMove) {
			board->orangeEscaped = true;
		} else {
			board->blackEscaped = true;
		}
	}
	toMove[endY * 10 + endX] = toMove[startY * 10 + startX];
	toMove[startY * 10 + startX] = EMPTY;
	opponent[endY * 10 + endX] = EMPTY;
	board->orangeToMove = !board->orangeToMove;
}
