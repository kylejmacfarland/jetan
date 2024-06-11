#include "array.h"

#include <stddef.h>

void initArray(Array* a, unsigned int startSize, unsigned int eSize) {
	a->elementSize = eSize;
	a->array = malloc(startSize * a->elementSize);
	a->size = 0;
	a->maxSize = startSize;
}

void addElement(Array* a, void* element) {
	for (int i = 0; i < a->size; i++) {
		if (a->array[i] == NULL) {
			a->array[i] = element;
			return;
		}
	}
	if (a->size > a->maxSize) {
		a->maxSize *= 2;
		a->array = realloc(a->array, a->maxSize * a->elementSize);
	}
	a->array[a->size++] = element;
}

void clearArray(Array* a) {
	for (int i = 0; i < a->size; i++) {
		a->array[i] = NULL;
	}
	a->size = 0;
}

void freeArray(Array* a) {
	free(a->array);
	a->array = NULL;
	a->size = 0;
	a->maxSize = 0;
}
