#include "board.h"

#include "array.h"

void setupBoard(Board* board) {
	for (int i = 0; i < 100; i++) {
		board->orange[i] = EMPTY;
		board->black[i] = EMPTY;
	}
	board->orange[0] = WARRIOR;
	board->orange[1] = PADWAR;
	board->orange[2] = DWAR;
	board->orange[3] = FLIER;
	board->orange[4] = PRINCESS;
	board->orange[5] = CHIEF;
	board->orange[6] = FLIER;
	board->orange[7] = DWAR;
	board->orange[8] = PADWAR;
	board->orange[9] = WARRIOR;
	board->orange[10] = THOAT;
	board->orange[11] = PANTHAN;
	board->orange[12] = PANTHAN;
	board->orange[13] = PANTHAN;
	board->orange[14] = PANTHAN;
	board->orange[15] = PANTHAN;
	board->orange[16] = PANTHAN;
	board->orange[17] = PANTHAN;
	board->orange[18] = PANTHAN;
	board->orange[19] = THOAT;
	board->black[80] = THOAT;
	board->black[81] = PANTHAN;
	board->black[82] = PANTHAN;
	board->black[83] = PANTHAN;
	board->black[84] = PANTHAN;
	board->black[85] = PANTHAN;
	board->black[86] = PANTHAN;
	board->black[87] = PANTHAN;
	board->black[88] = PANTHAN;
	board->black[89] = THOAT;
	board->black[90] = WARRIOR;
	board->black[91] = PADWAR;
	board->black[92] = DWAR;
	board->black[93] = FLIER;
	board->black[94] = CHIEF;
	board->black[95] = PRINCESS;
	board->black[96] = FLIER;
	board->black[97] = DWAR;
	board->black[98] = PADWAR;
	board->black[99] = WARRIOR;
	board->orangeToMove = true;
	board->orangeEscaped = true;
	board->blackEscaped = false;
	return true;
}

unsigned char pieceAt(char* side, int x, int y) {
	if (x < 0 || y < 0 || x >= 10 || y >= 10) {
		return INVALID;
	}
	return side[y * 10 + x];
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