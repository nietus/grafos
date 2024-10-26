#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define A 0
#define B 1
#define C 2
#define O 1000000000

typedef struct Edge {
    int to;
    int capacity;
    int flow;
    struct Edge *next;
} Edge;

// Ponteiro para a lista de adjacência
Edge **adj;
int head, tail;

// Arrays dinâmicos para color e predecessor
int *color_arr;
int *pred_arr;

int n, e;

// Função para encontrar o mínimo entre dois inteiros
int min_value(int x, int y) {
    return x < y ? x : y;
}

int head, tail;
int *q;

// Enfileira um vértice na fila
void enqueue(int x) {
    q[tail++] = x;
    color_arr[x] = B;
}

// Desenfileira um vértice da fila
int dequeue() {
    int x = q[head++];
    color_arr[x] = C;
    return x;
}

// Adiciona uma aresta no grafo
void addEdge(int u, int v, int cap) {
    // Adiciona aresta direta
    Edge *e1 = (Edge *)malloc(sizeof(Edge));
    if (e1 == NULL) {
        fprintf(stderr, "Erro de alocação de memória para aresta.\n");
        exit(EXIT_FAILURE);
    }
    e1->to = v;
    e1->capacity = cap;
    e1->flow = 0;
    e1->next = adj[u];
    adj[u] = e1;

    // Adiciona aresta reversa
    Edge *e2 = (Edge *)malloc(sizeof(Edge));
    if (e2 == NULL) {
        fprintf(stderr, "Erro de alocação de memória para aresta reversa.\n");
        exit(EXIT_FAILURE);
    }
    e2->to = u;
    e2->capacity = 0;
    e2->flow = 0;
    e2->next = adj[v];
    adj[v] = e2;
}

// BFS para encontrar caminho aumentante
int bfs(int start, int target) {
    for(int u = 0; u < n; u++) {
        color_arr[u] = A;
    }
    head = tail = 0;
    enqueue(start);
    pred_arr[start] = -1;

    while(head != tail) {
        int u = dequeue();
        for(Edge *e = adj[u]; e != NULL; e = e->next) {
            int v = e->to;
            if(color_arr[v] == A && e->capacity - e->flow > 0) {
                enqueue(v);
                pred_arr[v] = u;
                if(v == target) return 1; // Alvo encontrado
            }
        }
    }
    return color_arr[target] == C;
}

// Algoritmo Ford-Fulkerson para fluxo máximo
int fordFulkerson(int source, int sink) {
    int max_flow = 0;
    while(bfs(source, sink)) {
        int increment = O;
        int u = sink;
        // Encontrar o fluxo mínimo no caminho encontrado
        while(pred_arr[u] != -1) {
            int parent = pred_arr[u];
            Edge *e;
            // Encontrar a aresta parent -> u
            for(e = adj[parent]; e != NULL; e = e->next) {
                if(e->to == u) break;
            }
            if(e == NULL) {
                increment = 0;
                break;
            }
            if(min_value(e->capacity - e->flow, increment) == e->capacity - e->flow) {
                increment = e->capacity - e->flow;
            }
            u = parent;
        }

        if(increment == 0) break;

        max_flow += increment;

        u = sink;
        // Atualizar os fluxos ao longo do caminho
        while(pred_arr[u] != -1) {
            int parent = pred_arr[u];
            Edge *e;
            // Atualizar fluxo da aresta parent -> u
            for(e = adj[parent]; e != NULL; e = e->next) {
                if(e->to == u) break;
            }
            if(e != NULL) {
                e->flow += increment;
            }
            // Atualizar fluxo da aresta reversa u -> parent
            for(e = adj[u]; e != NULL; e = e->next) {
                if(e->to == parent) break;
            }
            if(e != NULL) {
                e->flow -= increment;
            }
            u = parent;
        }
    }
    return max_flow;
}

// Função para ler o grafo de um arquivo e alocar dinamicamente as estruturas
void readGraphFromFileDynamic(const char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s!\n", filename);
        exit(EXIT_FAILURE);
    }

    if(fscanf(file, "%d %d", &n, &e) != 2) {
        fprintf(stderr, "Erro ao ler o número de nós e arestas do arquivo.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    printf("Número de nós: %d\nNúmero de arestas: %d\n", n, e);

    // Alocar listas de adjacência
    adj = (Edge **)calloc(n, sizeof(Edge *));
    if(adj == NULL) {
        fprintf(stderr, "Erro ao alocar memória para listas de adjacência.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Alocar arrays dinâmicos
    color_arr = (int *)malloc(n * sizeof(int));
    pred_arr = (int *)malloc(n * sizeof(int));
    q = (int *)malloc(n * sizeof(int));
    if(color_arr == NULL || pred_arr == NULL || q == NULL) {
        fprintf(stderr, "Erro ao alocar memória para arrays.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < e; i++) {
        int u, v, cap;
        if(fscanf(file, "%d %d %d", &u, &v, &cap) != 3) {
            fprintf(stderr, "Erro ao ler a aresta %d do arquivo.\n", i + 1);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        if(u < 0 || u >= n || v < 0 || v >= n) {
            fprintf(stderr, "Aresta %d tem nós inválidos: %d -> %d\n", i + 1, u, v);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        addEdge(u, v, cap);
    }

    fclose(file);
}

// Função para extrair e listar os caminhos disjuntos em arestas
void extractPaths(int source, int sink, int max_flow) {
    FILE *path_file = fopen("paths.txt", "w");
    if(path_file == NULL) {
        fprintf(stderr, "Erro ao criar o arquivo de caminhos.\n");
        return;
    }

    fprintf(path_file, "Caminhos Disjuntos em Arestas:\n");
    fprintf(path_file, "Total de Caminhos: %d\n", max_flow);

    // Para evitar usar uma matriz de fluxo enorme, vamos percorrer as listas de adjacência
    for(int i = 0; i < max_flow; i++) {
        int current = source;
        fprintf(path_file, "Caminho %d: %d", i + 1, current);
        while(current != sink) {
            Edge *e = adj[current];
            int next = -1;
            while(e != NULL) {
                if(e->flow > 0) {
                    next = e->to;
                    e->flow--; // Remover a aresta usada
                    break;
                }
                e = e->next;
            }
            if(next == -1) {
                fprintf(path_file, " -> N/A");
                break;
            }
            fprintf(path_file, " -> %d", next);
            current = next;
        }
        fprintf(path_file, "\n");
    }

    fclose(path_file);
}

int main() {
    // Inicializar as listas de adjacência como NULL (será alocado na leitura)
    adj = NULL;

    // Opção para escolher o arquivo de entrada
    int escolha_arquivo;
    printf("Escolha o arquivo de entrada:\n");
    printf("1. graph.txt (Aleatorio)\n");
    printf("2. graph_grid.txt (Grade)\n");
    printf("Digite 1 ou 2: ");
    if(scanf("%d", &escolha_arquivo) != 1) {
        fprintf(stderr, "Entrada inválida.\n");
        exit(EXIT_FAILURE);
    }

    char *filename;
    if(escolha_arquivo == 1) {
        filename = "graph.txt";
    }
    else if(escolha_arquivo == 2) {
        filename = "graph_grid.txt";
    }
    else {
        fprintf(stderr, "Escolha inválida. Encerrando o programa.\n");
        exit(EXIT_FAILURE);
    }

    // Le o grafo do arquivo selecionado
    readGraphFromFileDynamic(filename);

    int s_default = 0;
    int t_default = n - 1;
    int s, t;
    char escolha;

    // Pergunta ao usuário se deseja usar os valores padrão
    printf("Deseja usar os valores padrão de origem e destino? (%d e %d) [S/N]: ", s_default, t_default);
    if(scanf(" %c", &escolha) != 1) { // Espaço antes de %c para ignorar caracteres anteriores
        fprintf(stderr, "Entrada inválida.\n");
        exit(EXIT_FAILURE);
    }

    if(escolha == 'S' || escolha == 's') {
        s = s_default;
        t = t_default;
        printf("Usando origem = %d e destino = %d\n", s, t);
    }
    else {
        printf("Digite o vértice de origem e o vértice de destino: ");
        if(scanf("%d %d", &s, &t) != 2) {
            fprintf(stderr, "Entrada inválida.\n");
            exit(EXIT_FAILURE);
        }

        // Validação dos vértices
        if(s < 0 || s >= n || t < 0 || t >= n) {
            fprintf(stderr, "Vértices de origem ou destino inválidos.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Iniciar a contagem do tempo para o fluxo máximo usando QueryPerformanceCounter
    LARGE_INTEGER frequency;
    LARGE_INTEGER start_flow, end_flow;
    double cpu_time_used_flow;

    if(!QueryPerformanceFrequency(&frequency)) {
        fprintf(stderr, "QueryPerformanceFrequency falhou!\n");
        exit(EXIT_FAILURE);
    }

    QueryPerformanceCounter(&start_flow);
    printf("Calculando o fluxo máximo de %d para %d...\n", s, t);
    int max_flow = fordFulkerson(s, t);
    QueryPerformanceCounter(&end_flow);

    cpu_time_used_flow = (double)(end_flow.QuadPart - start_flow.QuadPart) / frequency.QuadPart;
    printf("Fluxo Máximo: %d\n", max_flow);
    printf("Tempo para calcular o fluxo máximo: %.9f segundos\n", cpu_time_used_flow);

    // Iniciar a contagem do tempo para extrair os caminhos usando QueryPerformanceCounter
    if(max_flow > 0) {
        LARGE_INTEGER start_extract, end_extract;
        double cpu_time_used_extract;

        QueryPerformanceCounter(&start_extract);
        extractPaths(s, t, max_flow);
        QueryPerformanceCounter(&end_extract);

        cpu_time_used_extract = (double)(end_extract.QuadPart - start_extract.QuadPart) / frequency.QuadPart;
        printf("Caminhos listados em 'paths.txt'\n");
        printf("Tempo para extrair os caminhos: %.9f segundos\n", cpu_time_used_extract);
    }
    else {
        printf("Nenhum caminho disjunto em arestas encontrado entre %d e %d.\n", s, t);
    }

    // Libera a memória alocada para listas de adjacência
    for(int u = 0; u < n; u++) {
        Edge *e = adj[u];
        while(e != NULL) {
            Edge *temp = e;
            e = e->next;
            free(temp);
        }
    }
    free(adj);
    free(color_arr);
    free(pred_arr);
    free(q);

    return 0;
}
