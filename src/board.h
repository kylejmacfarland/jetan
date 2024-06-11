#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

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

void setupBoard(Board* board);

unsigned char pieceAt(char* side, int x, int y);

void displayBoard(Board* board);

void copyBoard(Board* dst, Board* src);

#endif
