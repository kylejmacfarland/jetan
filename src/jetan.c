#include <stdio.h>
#include <time.h>

#include "logic.h"
#include "array.h"
#include "util.h"

Board* current;

void playGame() {
	displayBoard(current);
	while (true) {
		makeMove(current);
		displayBoard(current);
		if (checkLoss(current)) {
			if (current->orangeToMove) {
				printf("Black wins!\n");
			} else {
				printf("Orange wins!\n");
			}
			return;
		}
	}
}

void main() {
	current = malloc(sizeof(Board));
	setRandomSeed(time(NULL));
	setupBoard(current);
	playGame();
	free(current);
}
