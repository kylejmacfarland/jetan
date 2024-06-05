#include "util.h"

unsigned int seed;

void setRandomSeed(unsigned int s) {
	seed = s;
}

unsigned int randi() {
	unsigned int x = seed;
	seed = x * 6264136223846793005 + 1442695040888963407;
	x ^= x >> 18;
	int r = x >> 59;
	return x >> r | x << (-r & 31);
}