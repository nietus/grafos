#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Transporte usando dual-matrix

#define MAX 10

typedef struct {
    int m; // Número de pontos de oferta
    int n; // Número de pontos de demanda
    int oferta[MAX];
    int demanda[MAX];
    int custo[MAX][MAX];
} Transporte;

int carregarDados(const char* nomeArquivo, Transporte* problema) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    // Lê número de pontos de oferta e demanda
    fscanf(arquivo, "%d %d", &problema->m, &problema->n);

    // Lê valores de oferta
    for (int i = 0; i < problema->m; i++) {
        fscanf(arquivo, "%d", &problema->oferta[i]);
    }

    // Lê valores de demanda
    for (int j = 0; j < problema->n; j++) {
        fscanf(arquivo, "%d", &problema->demanda[j]);
    }

    // Lê a matriz de custos
    for (int i = 0; i < problema->m; i++) {
        for (int j = 0; j < problema->n; j++) {
            fscanf(arquivo, "%d", &problema->custo[i][j]);
        }
    }

    fclose(arquivo);
    return 1;
}

// Função para resolver o problema pelo Método de Custo Mínimo
int resolverMinimoCusto(const Transporte* problema) {
    int m = problema->m;
    int n = problema->n;
    int oferta[MAX], demanda[MAX];
    int transporte[MAX][MAX] = {0};
    int custoTotal = 0;

    // Copia oferta e demanda para arrays locais para não modificar os originais
    for (int i = 0; i < m; i++) oferta[i] = problema->oferta[i];
    for (int j = 0; j < n; j++) demanda[j] = problema->demanda[j];

    // Alocação pelo método de custo mínimo
    while (1) {
        int minCusto = INT_MAX;
        int minI = -1, minJ = -1;

        // Encontrar a célula com o menor custo disponível
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (oferta[i] > 0 && demanda[j] > 0 && problema->custo[i][j] < minCusto) {
                    minCusto = problema->custo[i][j];
                    minI = i;
                    minJ = j;
                }
            }
        }

        // Se nenhuma célula válida for encontrada
        if (minI == -1 || minJ == -1) break;

        // Alocar a quantidade mínima entre oferta e demanda
        int quantidade = (oferta[minI] < demanda[minJ]) ? oferta[minI] : demanda[minJ];
        transporte[minI][minJ] = quantidade;
        oferta[minI] -= quantidade;
        demanda[minJ] -= quantidade;
        custoTotal += quantidade * problema->custo[minI][minJ];
    }

    printf("Custo Total do Transporte: %d\n", custoTotal);
    printf("Distribuição de Transporte:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (transporte[i][j] > 0) {
                printf("De %d para %d: %d unidades\n", i + 1, j + 1, transporte[i][j]);
            }
        }
    }

    return custoTotal;
}

int main() {
    Transporte problema;
    char nomeArquivo[100] = "data/transporte3.txt";
    if (!carregarDados(nomeArquivo, &problema)) {
        printf("Erro ao carregar os dados do problema.\n");
        return 1;
    }

    resolverMinimoCusto(&problema);

    return 0;
}
