#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "logic.h"
#include "visual.h"
#include "array.h"
#include "util.h"

const int FPS = 10;

Board* current;

void playGame(Board* board) {
	while (true) {
		makeMove(board);
		if (checkLoss(board)) {
			if (board->orangeToMove) {
				printf("Black wins!\n");
			} else {
				printf("Orange wins!\n");
			}
			return;
		}
	}
}

void handleWindow() {
	bool running = true;
	int startTicks, ticksElapsed;
	while (true) {
		startTicks = SDL_GetTicks();
		renderBoard(current);
		if (isQuitRequested()) {
			return;
		}
		ticksElapsed = SDL_GetTicks() - startTicks;
		if (ticksElapsed < 1000 / FPS) {
			SDL_Delay(1000 / FPS - ticksElapsed);
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
	pthread_t logicThread;
	pthread_create(&logicThread, NULL, playGame, current);
	handleWindow();
	free(current);
	destroyVisual();
	return 0;
}
