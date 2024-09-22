# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -g

# Arquivos de objeto
OBJ = main.o graph.o dfs.o stack.o

# Nome do executável
EXEC = grafo

# Regra padrão para compilar o programa
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

# Compilando main.c
main.o: main.c graph.h dfs.h
	$(CC) $(CFLAGS) -c main.c

# Compilando graph.c
graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c

# Compilando dfs.c
dfs.o: dfs.c dfs.h graph.h stack.h
	$(CC) $(CFLAGS) -c dfs.c

# Compilando stack.c
stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

# Limpeza dos arquivos temporários
clean:
	rm -f $(OBJ) $(EXEC)
