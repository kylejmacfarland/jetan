#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#include "array.h"
#include "util.h"

#define PANTHAN 0x01
#define THOAT 0x01 << 1
#define WARRIOR 0x01 << 2
#define PADWAR 0x01 << 3
#define DWAR 0x01 << 4
#define FLIER 0x01 << 5
#define PRINCESS 0x01 << 6
#define CHIEF 0x01 << 7
#define EMPTY 0x00
#define INVALID 0xFF

typedef struct {
	unsigned char orange[100];
	unsigned char black[100];
	bool orangeToMove;
	bool orangeEscaped;
	bool blackEscaped;
} Board;

typedef struct {
	int startX;
	int startY;
	int endX;
	int endY;
} Move;

Board current;
bool loss = false;

void setup() {
	for (int i = 0; i < 100; i++) {
		current.orange[i] = EMPTY;
		current.black[i] = EMPTY;
	}
	current.orange[0] = WARRIOR;
	current.orange[1] = PADWAR;
	current.orange[2] = DWAR;
	current.orange[3] = FLIER;
	current.orange[4] = PRINCESS;
	current.orange[5] = CHIEF;
	current.orange[6] = FLIER;
	current.orange[7] = DWAR;
	current.orange[8] = PADWAR;
	current.orange[9] = WARRIOR;
	current.orange[10] = THOAT;
	current.orange[11] = PANTHAN;
	current.orange[12] = PANTHAN;
	current.orange[13] = PANTHAN;
	current.orange[14] = PANTHAN;
	current.orange[15] = PANTHAN;
	current.orange[16] = PANTHAN;
	current.orange[17] = PANTHAN;
	current.orange[18] = PANTHAN;
	current.orange[19] = THOAT;
	current.black[80] = THOAT;
	current.black[81] = PANTHAN;
	current.black[82] = PANTHAN;
	current.black[83] = PANTHAN;
	current.black[84] = PANTHAN;
	current.black[85] = PANTHAN;
	current.black[86] = PANTHAN;
	current.black[87] = PANTHAN;
	current.black[88] = PANTHAN;
	current.black[89] = THOAT;
	current.black[90] = WARRIOR;
	current.black[91] = PADWAR;
	current.black[92] = DWAR;
	current.black[93] = FLIER;
	current.black[94] = CHIEF;
	current.black[95] = PRINCESS;
	current.black[96] = FLIER;
	current.black[97] = DWAR;
	current.black[98] = PADWAR;
	current.black[99] = WARRIOR;
	current.orangeToMove = true;
	current.orangeEscaped = true;
	current.blackEscaped = false;
}

unsigned char pieceAt(char* side, int x, int y) {
	if (x < 0 || y < 0 || x >= 10 || y >= 10) {
		return INVALID;
	}
	return side[y * 10 + x];
}

void displayBoard(Board* board) {
	printf("=============================\n");
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			bool noPiece = false;
			switch (pieceAt(board->orange, x, y)) {
			case PANTHAN:
				printf("\033[0;31m");
				printf("PA ");
				printf("\033[0m");
				break;
			case THOAT:
				printf("\033[0;31m");
				printf("TH ");
				printf("\033[0m");
				break;
			case WARRIOR:
				printf("\033[0;31m");
				printf("WA ");
				printf("\033[0m");
				break;
			case PADWAR:
				printf("\033[0;31m");
				printf("PW ");
				printf("\033[0m");
				break;
			case DWAR:
				printf("\033[0;31m");
				printf("DW ");
				printf("\033[0m");
				break;
			case FLIER:
				printf("\033[0;31m");
				printf("FL ");
				printf("\033[0m");
				break;
			case PRINCESS:
				printf("\033[0;31m");
				printf("PR ");
				printf("\033[0m");
				break;
			case CHIEF:
				printf("\033[0;31m");
				printf("CH ");
				printf("\033[0m");
				break;
			default:
				noPiece = true;
				break;
			}
			if (noPiece) {
				noPiece = false;
				switch (pieceAt(board->black, x, y)) {
				case PANTHAN:
					printf("PA ");
					break;
				case THOAT:
					printf("TH ");
					break;
				case WARRIOR:
					printf("WA ");
					break;
				case PADWAR:
					printf("PW ");
					break;
				case DWAR:
					printf("DW ");
					break;
				case FLIER:
					printf("FL ");
					break;
				case PRINCESS:
					printf("PR ");
					break;
				case CHIEF:
					printf("CH ");
					break;
				default:
					noPiece = true;
				}
				if (noPiece) {
					if ((x + y) % 2) {
						printf(".. ");
					} else {
						printf("   ");
					}
				}
			}
		}
		printf("\n");
	}
	printf("=============================\n");
}

void copyBoard(Board* dst, Board* src) {
	for (int i = 0; i < 100; i++) {
		dst->orange[i] = src->orange[i];
		dst->black[i] = src->black[i];
	}
	dst->orangeToMove = src->orangeToMove;
	dst->orangeEscaped = src->orangeEscaped;
	dst->blackEscaped = src->blackEscaped;
}

bool isSpaceThreatened(Board* board, bool checkOrangeThreat, int spaceX, int spaceY) {
	char* sideA = board->black;
	char* sideB = board->orange;
	if (checkOrangeThreat) {
		sideA = board->orange;
		sideB = board->black;
	}
	Array moves;
	initArray(&moves, 32);
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
			return 9999;
		} else {
			return -9999;
		}
	}
	if (depth == 0) {
		return evaluate(board);
	}
	Array moves;
	initArray(&moves, 32);
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
	initArray(&possibleMoves, 32);
	getMoves(board, &possibleMoves);

	// Evaluate moves
	Array bestMoves;
	initArray(&bestMoves, 4);
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
			addElement(&bestMoves, possibleMoves.array[i]);
		} else if (n == max) {
			addElement(&bestMoves, possibleMoves.array[i]);
		}
	}
	free(b);

	if (noLegalMoves) {
		loss = true;
	} else {
		// Make move
		Move* m = (Move*) bestMoves.array[randi() % bestMoves.size];
		move(board, m->startX, m->startY, m->endX, m->endY);
	}

	// Free memory
	for (int i = 0; i < possibleMoves.size; i++) {
		free(possibleMoves.array[i]);
	}
	freeArray(&possibleMoves);
	freeArray(&bestMoves);
}

void playGame() {
	displayBoard(&current);
	while (true) {
		makeMove(&current);
		displayBoard(&current);
		if (checkLoss(&current)) {
			if (current.orangeToMove) {
				printf("Black wins!\n");
			} else {
				printf("Orange wins!\n");
			}
			return;
		}
	}
}

void main() {
	setRandomSeed(time(NULL));
	setup();
	playGame();
}
