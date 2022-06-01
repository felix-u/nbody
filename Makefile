CFLAGS=-Wextra -Wall -O3
# LIBS=`pkg-config --cflags`
LIBS=-lraylib -lm -lpthread -ldl -lrt

nbody: main.c
	$(CC) $(CFLAGS) $(LIBS) -o nbody main.c

debug: main.c
	$(CC) -g $(CFLAGS) $(LIBS) -o nbody main.c
