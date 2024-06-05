#ifndef ARRAY_H
#define ARRAY_H

typedef struct {
	void** array;
	int size;
	int maxSize;
} Array;

void initArray(Array* a, int startSize);

void addElement(Array* a, void* element);

void clearArray(Array* a);

void freeArray(Array* a);

#endif
