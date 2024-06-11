#include "visual.h"

#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 380;
const int SCREEN_HEIGHT = 380;
const int SQUARE_SIZE = 38;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* icon;
SDL_Texture* pieceTexture;
SDL_Event event;

SDL_Texture* loadTexture(char* path) {
	SDL_Surface* surface = SDL_LoadBMP(path);
	if (surface == NULL) {
		printf("Unable to load image \"%s\". SDL_Error: %s\n", path, SDL_GetError());
		return NULL;
	}
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0x00, 0xFF));
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (tex == NULL) {
		printf("Unable to create texture from \"%s\". SDL_Error: %s\n", path, SDL_GetError());
		return NULL;
	}
	return tex;
}

bool initVisual() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	window = SDL_CreateWindow("Jetan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Could not create window. SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	icon = SDL_LoadBMP("../res/icon.bmp");
	SDL_SetWindowIcon(window, icon);
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("Could not create renderer. SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	pieceTexture = loadTexture("../res/piece.bmp");
	if (pieceTexture == NULL) {
		return false;
	}
	return true;
}

bool isQuitRequested() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return true;
		}
	}
	return false;
}

void renderPiece(int pieceIndex, int x, int y, bool isOrange) {
	SDL_Rect srcrect = {pieceIndex * 19, (!isOrange) * 19, 19, 19};
	SDL_Rect dstrect = {x * SQUARE_SIZE, y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
	SDL_RenderCopy(renderer, pieceTexture, &srcrect, &dstrect);
}

void renderBoard(Board* board) {
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);
	SDL_Rect rect = {0, 0, SQUARE_SIZE, SQUARE_SIZE};
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			if ((x + y) % 2 == 0) {	
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x00, 0xFF);
			} else {
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			}
			rect.x = x * SQUARE_SIZE;
			rect.y = y * SQUARE_SIZE;
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			bool noPiece = false;
			switch (pieceAt(board->orange, x, y)) {
			case PANTHAN:
				renderPiece(0, x, y, true);
				break;
			case THOAT:
				renderPiece(1, x, y, true);
				break;
			case WARRIOR:
				renderPiece(2, x, y, true);
				break;
			case PADWAR:
				renderPiece(3, x, y, true);
				break;
			case DWAR:
				renderPiece(4, x, y, true);
				break;
			case FLIER:
				renderPiece(5, x, y, true);
				break;
			case PRINCESS:
				renderPiece(7, x, y, true);
				break;
			case CHIEF:
				renderPiece(6, x, y, true);
				break;
			default:
				noPiece = true;
				break;
			}
			if (noPiece) 
				switch (pieceAt(board->black, x, y)) {
				case PANTHAN:
					renderPiece(0, x, y, false);
					break;
				case THOAT:
					renderPiece(1, x, y, false);
					break;
				case WARRIOR:
					renderPiece(2, x, y, false);
					break;
				case PADWAR:
					renderPiece(3, x, y, false);
					break;
				case DWAR:
					renderPiece(4, x, y, false);
					break;
				case FLIER:
					renderPiece(5, x, y, false);
					break;
				case PRINCESS:
					renderPiece(7, x, y, false);
					break;
				case CHIEF:
					renderPiece(6, x, y, false);
					break;
				}
			}
		}
	SDL_RenderPresent(renderer);
}

void destroyVisual() {
	SDL_FreeSurface(icon);
	SDL_DestroyTexture(pieceTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}