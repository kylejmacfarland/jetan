OBJS = jetan.c array.c util.c
CC = gcc
COMPILER_FLAGS = -w
OBJ_NAME = jetan

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)
