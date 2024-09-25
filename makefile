# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
DEPS = graph.h dfs.h
OBJ = main.o graph.o dfs.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

grafo: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o grafo
