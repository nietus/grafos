// gerador.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CAPACITY 1

// Função para gerar um grafo aleatório garantindo conectividade
void generateGraph(int nodes, int edges, const char *filename) {
    int i, u, v, capacity;
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita.\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d %d\n", nodes, edges);

    srand(time(NULL));

    // Garantir conectividade adicionando um caminho sequencial de 0 a n-1
    for (i = 0; i < nodes - 1; i++) {
        fprintf(file, "%d %d %d\n", i, i + 1, 1);
    }

    // Adicionar arestas aleatórias restantes
    for (i = nodes - 1; i < edges; i++) {
        u = rand() % nodes;
        v = rand() % nodes;
        while (v == u) { // Evitar loops
            v = rand() % nodes;
        }
        capacity = (rand() % MAX_CAPACITY) + 1; // Capacidade aleatória (1)
        fprintf(file, "%d %d %d\n", u, v, capacity);
    }

    fclose(file);
    printf("Grafo gerado e salvo em '%s'\n", filename);
}

int main(int argc, char *argv[]) {
    int nodes, edges;
    char *filename = "graph.txt";

    // Verificar se o número correto de argumentos foi fornecido
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <numero_de_nos> <numero_de_arestas>\n", argv[0]);
        return EXIT_FAILURE;
    }

    nodes = atoi(argv[1]);
    edges = atoi(argv[2]);

    // Validar entradas
    if (nodes <= 0) {
        fprintf(stderr, "Erro: O numero de nos deve ser positivo.\n");
        return EXIT_FAILURE;
    }

    if (edges < nodes - 1) {
        fprintf(stderr, "Erro: Numero de arestas insuficiente para garantir conectividade (minimo: %d).\n", nodes - 1);
        return EXIT_FAILURE;
    }

    if (edges > (nodes * (nodes - 1))) {
        fprintf(stderr, "Erro: Numero de arestas excede o maximo permitido para um grafo direcionado simples (maximo: %d).\n", nodes * (nodes - 1));
        return EXIT_FAILURE;
    }

    generateGraph(nodes, edges, filename);

    return EXIT_SUCCESS;
}
