#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX 10

typedef struct {
    int m; // Número de fornecedores
    int n; // Número de consumidores
    int oferta[MAX];
    int demanda[MAX];
    int custo[MAX][MAX];
    int dual[MAX][MAX];
} Transporte;

// Função para ler os dados do usuário
int lerDados(Transporte* problema) {
    printf("=== Problema de Transporte ===\n");
    printf("Digite o numero de fornecedores (maximo %d): ", MAX);
    scanf("%d", &problema->m);
    if (problema->m > MAX || problema->m <= 0) {
        printf("Número de fornecedores inválido.\n");
        return 0;
    }

    printf("Digite o numero de consumidores (maximo %d): ", MAX);
    scanf("%d", &problema->n);
    if (problema->n > MAX || problema->n <= 0) {
        printf("Número de consumidores inválido.\n");
        return 0;
    }

    printf("\nDigite as ofertas para cada fornecedor:\n");
    for (int i = 0; i < problema->m; i++) {
        printf("Oferta F%d: ", i + 1);
        scanf("%d", &problema->oferta[i]);
        if (problema->oferta[i] < 0) {
            printf("Oferta não pode ser negativa.\n");
            return 0;
        }
    }

    printf("\nDigite as demandas para cada consumidor:\n");
    for (int j = 0; j < problema->n; j++) {
        printf("Demanda C%d: ", j + 1);
        scanf("%d", &problema->demanda[j]);
        if (problema->demanda[j] < 0) {
            printf("Demanda não pode ser negativa.\n");
            return 0;
        }
    }

    printf("\nDigite a matriz de custos:\n");
    for (int i = 0; i < problema->m; i++) {
        for (int j = 0; j < problema->n; j++) {
            printf("Custo F%d->C%d: ", i + 1, j + 1);
            scanf("%d", &problema->custo[i][j]);
            if (problema->custo[i][j] < 0) {
                printf("Custo não pode ser negativo.\n");
                return 0;
            }
        }
    }

    return 1;
}

// Função para balancear o problema adicionando linhas ou colunas fictícias
void balancearProblema(Transporte* problema) {
    int totalOferta = 0, totalDemanda = 0;
    for (int i = 0; i < problema->m; i++) {
        totalOferta += problema->oferta[i];
    }
    for (int j = 0; j < problema->n; j++) {
        totalDemanda += problema->demanda[j];
    }

    if (totalOferta > totalDemanda) {
        // Adicionar coluna fictícia
        int j = problema->n;
        problema->demanda[j] = totalOferta - totalDemanda;
        for (int i = 0; i < problema->m; i++) {
            problema->custo[i][j] = 0; // Custo zero para a coluna fictícia
            problema->dual[i][j] = 0;  // Inicializar dual matrix
        }
        problema->n += 1;
        printf("\nProblema nao estava balanceado. Adicionando coluna ficticia C%d com demanda %d.\n", j + 1, problema->demanda[j]);
    } else if (totalDemanda > totalOferta) {
        // Adicionar linha fictícia
        int i = problema->m;
        problema->oferta[i] = totalDemanda - totalOferta;
        for (int j = 0; j < problema->n; j++) {
            problema->custo[i][j] = 0; // Custo zero para a linha fictícia
            problema->dual[i][j] = 0;  // Inicializar dual matrix
        }
        problema->m += 1;
        printf("\nProblema nao estava balanceado. Adicionando linha ficticia F%d com oferta %d.\n", i + 1, problema->oferta[i]);
    } else {
        printf("\nO problema ja esta balanceado.\n");
    }

    // Inicializar matriz dual com zeros
    for (int i = 0; i < problema->m; i++) {
        for (int j = 0; j < problema->n; j++) {
            problema->dual[i][j] = 0;
        }
    }
}

// Função para calcular as penalidades de Vogel
void calcularPenalidadesVogel(int m, int n, int custo[MAX][MAX], int oferta[], int demanda[],
                              int penalidadesLinha[], int penalidadesColuna[]) {
    for (int i = 0; i < m; i++) {
        int min1 = INT_MAX, min2 = INT_MAX;
        for (int j = 0; j < n; j++) {
            if (oferta[i] > 0 && demanda[j] > 0) {
                if (custo[i][j] < min1) {
                    min2 = min1;
                    min1 = custo[i][j];
                } else if (custo[i][j] < min2) {
                    min2 = custo[i][j];
                }
            }
        }
        penalidadesLinha[i] = (min2 != INT_MAX) ? min2 - min1 : min1;
    }

    for (int j = 0; j < n; j++) {
        int min1 = INT_MAX, min2 = INT_MAX;
        for (int i = 0; i < m; i++) {
            if (oferta[i] > 0 && demanda[j] > 0) {
                if (custo[i][j] < min1) {
                    min2 = min1;
                    min1 = custo[i][j];
                } else if (custo[i][j] < min2) {
                    min2 = custo[i][j];
                }
            }
        }
        penalidadesColuna[j] = (min2 != INT_MAX) ? min2 - min1 : min1;
    }
}

// Função para inicializar a solução usando o Método das Penalidades de Vogel (VAM)
void inicializarSolucaoVAM(int m, int n, int oferta[], int demanda[], int transporte[MAX][MAX], int custo[MAX][MAX]) {
    // Inicializa transporte com zeros
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            transporte[i][j] = 0;
        }
    }

    int linhaAlocada[MAX] = {0};
    int colunaAlocada[MAX] = {0};

    while (1) {
        int penalidadesLinha[MAX];
        int penalidadesColuna[MAX];
        calcularPenalidadesVogel(m, n, custo, oferta, demanda, penalidadesLinha, penalidadesColuna);

        int maxPenalidade = -1;
        int tipo = 0; // 1 para linha, 2 para coluna
        int indice = -1;

        // Encontrar a linha com a maior penalidade
        for (int i = 0; i < m; i++) {
            if (!linhaAlocada[i] && penalidadesLinha[i] > maxPenalidade) {
                maxPenalidade = penalidadesLinha[i];
                tipo = 1;
                indice = i;
            }
        }

        // Encontrar a coluna com a maior penalidade
        for (int j = 0; j < n; j++) {
            if (!colunaAlocada[j] && penalidadesColuna[j] > maxPenalidade) {
                maxPenalidade = penalidadesColuna[j];
                tipo = 2;
                indice = j;
            }
        }

        if (maxPenalidade == -1)
            break; // Todas as linhas e colunas estão alocadas

        if (tipo == 1) { // Linha
            int minCusto = INT_MAX;
            int minJ = -1;
            for (int j = 0; j < n; j++) {
                if (!colunaAlocada[j] && custo[indice][j] < minCusto) {
                    minCusto = custo[indice][j];
                    minJ = j;
                }
            }

            if (minJ == -1)
                break; // Nenhuma coluna disponível

            int quantidade = (oferta[indice] < demanda[minJ]) ? oferta[indice] : demanda[minJ];
            transporte[indice][minJ] = quantidade;
            oferta[indice] -= quantidade;
            demanda[minJ] -= quantidade;

            if (oferta[indice] == 0 && demanda[minJ] == 0) {
                colunaAlocada[minJ] = 1;
            } else if (oferta[indice] == 0) {
                linhaAlocada[indice] = 1;
            } else {
                colunaAlocada[minJ] = 1;
            }
        } else if (tipo == 2) { // Coluna
            int minCusto = INT_MAX;
            int minI = -1;
            for (int i = 0; i < m; i++) {
                if (!linhaAlocada[i] && custo[i][indice] < minCusto) {
                    minCusto = custo[i][indice];
                    minI = i;
                }
            }

            if (minI == -1)
                break; // Nenhuma linha disponível

            int quantidade = (oferta[minI] < demanda[indice]) ? oferta[minI] : demanda[indice];
            transporte[minI][indice] = quantidade;
            oferta[minI] -= quantidade;
            demanda[indice] -= quantidade;

            if (oferta[minI] == 0 && demanda[indice] == 0) {
                linhaAlocada[minI] = 1;
            } else if (oferta[minI] == 0) {
                linhaAlocada[minI] = 1;
            } else {
                colunaAlocada[indice] = 1;
            }
        }
    }
}

// Função para calcular os multiplicadores duais u e v usando o Método MODI
int calcularMultiplicadoresMODI(int m, int n, int transporte[MAX][MAX], const int custo[MAX][MAX], int u[], int v[], Transporte* problema) {
    // Inicializa u e v como indefinidos
    for (int i = 0; i < m; i++)
        u[i] = INT_MIN;
    for (int j = 0; j < n; j++)
        v[j] = INT_MIN;

    u[0] = 0;

    int atualizado = 1;

    while (atualizado) {
        atualizado = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (transporte[i][j] > 0) {
                    if (u[i] != INT_MIN && v[j] == INT_MIN) {
                        v[j] = custo[i][j] - u[i];
                        atualizado = 1;
                    } else if (u[i] == INT_MIN && v[j] != INT_MIN) {
                        u[i] = custo[i][j] - v[j];
                        atualizado = 1;
                    }
                }
            }
        }
    }

    // Verifica se todos os multiplicadores duais foram determinados
    for (int i = 0; i < m; i++) {
        if (u[i] == INT_MIN)
            return 0;
    }
    for (int j = 0; j < n; j++) {
        if (v[j] == INT_MIN)
            return 0;
    }

    // Popula a matriz dual
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            problema->dual[i][j] = u[i] + v[j];
        }
    }

    return 1; // Sucesso
}

// Função para imprimir os multiplicadores duais (para depuração)
void imprimirMultiplicadores(int m, int n, int u[], int v[]) {
    printf("\nMultiplicadores Duais:\n");
    for (int i = 0; i < m; i++) {
        printf("u%d = %d\t", i + 1, u[i]);
    }
    printf("\n");
    for (int j = 0; j < n; j++) {
        printf("v%d = %d\t", j + 1, v[j]);
    }
    printf("\n");
}

// Função para verificar a otimalidade e identificar a célula de entrada
int verificarOtimalidadeMODI(int m, int n, int transporte[MAX][MAX], const int custo[MAX][MAX],
                             int u[], int v[], int* entradaI, int* entradaJ) {
    int otimo = 1;
    int custoReduzido;
    int menorCustoReduzido = 0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (transporte[i][j] == 0) { // Células não básicas
                custoReduzido = custo[i][j] - u[i] - v[j];
                if (custoReduzido < menorCustoReduzido) {
                    menorCustoReduzido = custoReduzido;
                    *entradaI = i;
                    *entradaJ = j;
                    otimo = 0;
                }
            }
        }
    }

    return otimo;
}

// Função auxiliar para encontrar um ciclo a partir da célula de entrada
void encontrarCiclo(int m, int n, int transporte[MAX][MAX], int entradaI, int entradaJ,
                    int visitadoLinha[MAX], int visitadoColuna[MAX],
                    int caminhoLinha[MAX * MAX], int caminhoColuna[MAX * MAX], int* tamanho) {
    int filaI[MAX * MAX], filaJ[MAX * MAX];
    int inicio = 0, fim = 0;

    filaI[fim] = entradaI;
    filaJ[fim] = entradaJ;
    fim++;

    while (inicio < fim) {
        int i = filaI[inicio];
        int j = filaJ[inicio];
        inicio++;

        caminhoLinha[*tamanho] = i;
        caminhoColuna[*tamanho] = j;
        (*tamanho)++;

        if ((*tamanho) % 2 == 1) {
            for (int jj = 0; jj < n; jj++) {
                if (jj != j && transporte[i][jj] > 0 && !visitadoColuna[jj]) {
                    visitadoColuna[jj] = 1;
                    filaI[fim] = i;
                    filaJ[fim] = jj;
                    fim++;
                    if (i == entradaI && jj == entradaJ && *tamanho > 2) {
                        return;
                    }
                }
            }
        } else {
            for (int ii = 0; ii < m; ii++) {
                if (ii != i && transporte[ii][j] > 0 && !visitadoLinha[ii]) {
                    visitadoLinha[ii] = 1;
                    filaI[fim] = ii;
                    filaJ[fim] = j;
                    fim++;
                    if (ii == entradaI && j == entradaJ && *tamanho > 2) {
                        return;
                    }
                }
            }
        }
    }
}

// Função para ajustar a solução com base na célula de entrada
void ajustarCicloCompleto(int m, int n, int transporte[MAX][MAX], int entradaI, int entradaJ) {
    int visitadoLinha[MAX] = {0};
    int visitadoColuna[MAX] = {0};
    int caminhoLinha[MAX * MAX];
    int caminhoColuna[MAX * MAX];
    int tamanho = 0;

    visitadoLinha[entradaI] = 1;
    visitadoColuna[entradaJ] = 1;

    encontrarCiclo(m, n, transporte, entradaI, entradaJ,
                  visitadoLinha, visitadoColuna,
                  caminhoLinha, caminhoColuna, &tamanho);

    if (tamanho < 4 || tamanho % 2 != 0) {
        printf("Erro: Ciclo não encontrado ou ciclo inválido.\n");
        return;
    }

    int minValor = INT_MAX;
    for (int k = 1; k < tamanho; k += 2) {
        int i = caminhoLinha[k];
        int j = caminhoColuna[k];
        if (transporte[i][j] < minValor) {
            minValor = transporte[i][j];
        }
    }

    for (int k = 0; k < tamanho; k++) {
        int i = caminhoLinha[k];
        int j = caminhoColuna[k];
        if (k % 2 == 0) {
            transporte[i][j] += minValor;
        } else {
            transporte[i][j] -= minValor;
        }
    }
}

// Função para imprimir a solução final
void imprimirSolucao(int m, int n, const int transporte[MAX][MAX], const int custo[MAX][MAX]) {
    printf("\nMatriz de Transporte ótima:\n");
    printf("      ");
    for (int j = 0; j < n; j++) {
        printf("C%d\t", j + 1);
    }
    printf("\n");

    for (int i = 0; i < m; i++) {
        printf("F%d\t", i + 1);
        for (int j = 0; j < n; j++) {
            printf("%d\t", transporte[i][j]);
        }
        printf("\n");
    }

    int custoTotal = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            custoTotal += transporte[i][j] * custo[i][j];
        }
    }

    printf("\nCusto Total: %d\n", custoTotal);
}

// Função para resolver o problema usando VAM e MODI com Dual Matrix
void resolverTransporte(Transporte* problema) {
    int m = problema->m;
    int n = problema->n;
    int oferta[MAX];
    int demanda[MAX];
    int transporte[MAX][MAX];
    int u[MAX], v[MAX];

    for (int i = 0; i < m; i++)
        oferta[i] = problema->oferta[i];
    for (int j = 0; j < n; j++)
        demanda[j] = problema->demanda[j];

    inicializarSolucaoVAM(m, n, oferta, demanda, transporte, problema->custo);

    while (1) {
        if (!calcularMultiplicadoresMODI(m, n, transporte, problema->custo, u, v, problema)) {
            printf("Warning: Nao foi possivel determinar todos os multiplicadores duais.\n");
            break;
        }

        // Imprimir multiplicadores para depuração
        imprimirMultiplicadores(m, n, u, v);

        int entradaI = -1, entradaJ = -1;
        if (verificarOtimalidadeMODI(m, n, transporte, problema->custo, u, v, &entradaI, &entradaJ)) {
            break; // Solução ótima encontrada
        }

        ajustarCicloCompleto(m, n, transporte, entradaI, entradaJ);
    }

    printf("\nSolucao otima encontrada com Metodo MODI.\n");
    imprimirSolucao(m, n, transporte, problema->custo);
}

int main() {
    Transporte problema;

    if (!lerDados(&problema)) {
        printf("Erro na entrada de dados.\n");
        return 1;
    }

    balancearProblema(&problema);
    resolverTransporte(&problema);

    return 0;
}
