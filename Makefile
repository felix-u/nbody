CFLAGS=-std=c99 -Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow -Wextra -Wall -O3
LIBS=-lSDL2

nbody: src/main.c
	$(CC) $(CFLAGS) $(LIBS) -o nbody src/main.c
