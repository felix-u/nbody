CFLAGS=-std=c99 -Wall -pedantic -Werror -Wshadow -Wstrict-aliasing \
	   -Wstrict-overflow -Wextra -Wall
DEBUGFLAGS=-g -Og
RELEASEFLAGS=-O3
LIBS=-lSDL2 -lm

nbody: src/*
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(LIBS) -o nbody src/main.c

release: src/*
	$(CC) $(CFLAGS) $(RELEASEFLAGS) $(LIBS) -o nbody src/main.c
