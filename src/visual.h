#ifndef VISUAL_H
#define VISUAL_H

#include "board.h"

bool initVisual();

bool isQuitRequested();

void renderBoard(Board* board);

void destroyVisual();

#endif