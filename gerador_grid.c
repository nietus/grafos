#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_CAPACITY 1

void generateGridGraph(int rows, int cols, const char *filename) {
    // Calcula o número de nós e arestas
    long long nodes = (long long)rows * (long long)cols;
    // Cada aresta horizontal interna: rows * (cols - 1)
    // Cada aresta vertical interna: cols * (rows - 1)
    long long edges = (long long)rows * (cols - 1) + (long long)cols * (rows - 1);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo '%s' para escrita.\n", filename);
        exit(EXIT_FAILURE);
    }

    // Escreve o número de nós e arestas na primeira linha
    fprintf(file, "%lld %lld\n", nodes, edges);

    // Itera por cada célula na grade para adicionar as arestas
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            long long u = (long long)i * cols + j; // Nó atual

            // Conecta ao vizinho da direita se não estiver na última coluna
            if(j < cols - 1) {
                long long v = u + 1;
                fprintf(file, "%lld %lld %d\n", u, v, MAX_CAPACITY);
            }

            // Conecta ao vizinho abaixo se não estiver na última linha
            if(i < rows - 1) {
                long long v = u + cols;
                fprintf(file, "%lld %lld %d\n", u, v, MAX_CAPACITY);
            }
        }
    }

    fclose(file);
    printf("Grafo em grade gerado e salvo em '%s'\n", filename);
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Uso: %s <numero_de_linhas> <numero_de_colunas>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Converte os argumentos de linha de comando para inteiros
    char *endptr;
    long input_rows = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || input_rows <= 0 || input_rows > INT32_MAX) {
        fprintf(stderr, "Erro: Número de linhas inválido '%s'. Deve ser um inteiro positivo.\n", argv[1]);
        return EXIT_FAILURE;
    }

    long input_cols = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || input_cols <= 0 || input_cols > INT32_MAX) {
        fprintf(stderr, "Erro: Número de colunas inválido '%s'. Deve ser um inteiro positivo.\n", argv[2]);
        return EXIT_FAILURE;
    }

    int rows = (int)input_rows;
    int cols = (int)input_cols;

    // Verifica se o cálculo de nós pode causar overflow
    if ((long long)rows * (long long)cols > LLONG_MAX) {
        fprintf(stderr, "Erro: O tamanho da grade é muito grande e causa overflow.\n");
        return EXIT_FAILURE;
    }

    // Determina o nome do arquivo de saída
    const char *filename = "graph_grid.txt"; // Nome padrão
    if (argc == 4) {
        filename = argv[3];
    }

    // Gera o grafo em grade
    generateGridGraph(rows, cols, filename);

    return EXIT_SUCCESS;
}