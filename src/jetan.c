#include <stdio.h>
#include <time.h>

#include "logic.h"
#include "visual.h"
#include "array.h"
#include "util.h"

Board* current;

void playGame() {
	renderBoard(current);
	while (!isQuitRequested()) {
		makeMove(current);
		renderBoard(current);
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

int main() {
	if (!initVisual()) {
		return 1;
	}
	setRandomSeed(time(NULL));
	current = malloc(sizeof(Board));
	setupBoard(current);
	playGame();
	free(current);
	destroyVisual();
	return 0;
}
