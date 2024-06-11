#ifndef ARRAY_H
#define ARRAY_H

typedef struct {
	void** array;
	unsigned int elementSize;
	unsigned int size;
	unsigned int maxSize;
} Array;

void initArray(Array* a, unsigned int startSize, unsigned int eSize);

void addElement(Array* a, void* element);

void clearArray(Array* a);

void freeArray(Array* a);

#endif
