// graph.c
#include "graph.h"

// Função para adicionar uma aresta ao grafo
void addEdge(Node* adjList[], int u, int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Erro na alocação de memória para novo nó.\n");
        exit(EXIT_FAILURE);
    }
    newNode->vertex = v;
    newNode->next = adjList[u];
    adjList[u] = newNode;
}

// Função para comparar dois inteiros (usada na ordenação)
int compare(const void* a, const void* b) {
    int int_a = *(int*)a;
    int int_b = *(int*)b;
    if (int_a < int_b)
        return -1;
    else if (int_a > int_b)
        return 1;
    else
        return 0;
}

// Função para liberar uma única lista de adjacência
void freeAdjList(Node* adjList[], int vertex) {
    Node* temp = adjList[vertex];
    while (temp) {
        Node* toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    adjList[vertex] = NULL;
}

// Função para ordenar as listas de adjacência em ordem crescente
void sortAdjacencyList(Node* adjList[], int numVertices) {
    for (int i = 1; i <= numVertices; i++) {
        // Primeiro, conta o número de adjacências
        int count = 0;
        Node* temp = adjList[i];
        while (temp) {
            count++;
            temp = temp->next;
        }

        if (count > 1) {
            // Coleta os vértices em um array
            int* vertices = (int*)malloc(count * sizeof(int));
            if (!vertices) {
                printf("Erro na alocação de memória para ordenação.\n");
                exit(EXIT_FAILURE);
            }

            temp = adjList[i];
            for (int j = 0; j < count; j++) {
                vertices[j] = temp->vertex;
                temp = temp->next;
            }

            // Ordena o array
            qsort(vertices, count, sizeof(int), compare);

            // Liberar a lista de adjacência atual
            freeAdjList(adjList, i);

            // Reconstrói a lista de adjacência ordenada
            adjList[i] = NULL;
            for (int j = count - 1; j >= 0; j--) { // Inserir em ordem reversa para manter ordem crescente
                Node* newNode = (Node*)malloc(sizeof(Node));
                if (!newNode) {
                    printf("Erro na alocação de memória para novo nó.\n");
                    exit(EXIT_FAILURE);
                }
                newNode->vertex = vertices[j];
                newNode->next = adjList[i];
                adjList[i] = newNode;
            }

            free(vertices);
        }
    }
}

// Função para liberar a memória da lista de adjacência
void freeAdjacencyList(Node* adjList[], int numVertices) {
    for (int i = 1; i <= numVertices; i++) {
        freeAdjList(adjList, i);
    }
}

// Função para obter os predecessores de um vértice
int* getPredecessors(Node* adjList[], int numVertices, int vertex, int* count) {
    int* predecessors = NULL;
    *count = 0;

    for (int u = 1; u <= numVertices; u++) {
        Node* temp = adjList[u];
        while (temp) {
            if (temp->vertex == vertex) {
                int* tempPredecessors = (int*)realloc(predecessors, (*count + 1) * sizeof(int));
                if (!tempPredecessors) {
                    printf("Erro na alocação de memória para predecessores.\n");
                    free(predecessors);
                    exit(EXIT_FAILURE);
                }
                predecessors = tempPredecessors;
                predecessors[*count] = u;
                (*count)++;
            }
            temp = temp->next;
        }
    }

    return predecessors;
}
