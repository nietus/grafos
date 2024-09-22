#include "graph.h"

// Função para criar um novo nó na lista de adjacências
Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Erro na alocação de memória.\n");
        exit(1);
    }
    newNode->vertice = v;
    newNode->next = NULL;
    return newNode;
}

// Função para adicionar uma aresta ao grafo
void addEdge(Node* adjList[], int u, int v) {
    // Adicionando v à lista de adjacência de u
    Node* newNode = createNode(v);
    newNode->next = adjList[u];
    adjList[u] = newNode;
}

// Função para ordenar a lista de adjacência em ordem crescente (lexicográfica)
void sortAdjacencyList(Node* adjList[], int numVertices) {
    for (int i = 1; i <= numVertices; i++) {
        if (adjList[i] == NULL || adjList[i]->next == NULL)
            continue;

        // Converter a lista para um array para facilitar a ordenação
        int count = 0;
        Node* temp = adjList[i];
        while (temp) {
            count++;
            temp = temp->next;
        }

        int* arr = (int*)malloc(count * sizeof(int));
        if (!arr) {
            printf("Erro na alocação de memória para ordenação.\n");
            exit(1);
        }

        temp = adjList[i];
        for (int j = 0; j < count; j++) {
            arr[j] = temp->vertice;
            temp = temp->next;
        }

        // Ordenar o array usando Bubble Sort (para simplicidade)
        for (int j = 0; j < count - 1; j++) {
            for (int k = j + 1; k < count; k++) {
                if (arr[j] > arr[k]) {
                    int aux = arr[j];
                    arr[j] = arr[k];
                    arr[k] = aux;
                }
            }
        }

        // Reconstruir a lista ordenada
        // Liberar a lista existente
        temp = adjList[i];
        while (temp) {
            Node* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
        adjList[i] = NULL;

        // Adicionar os elementos ordenados à lista
        for (int j = count - 1; j >= 0; j--) {
            Node* newNode = createNode(arr[j]);
            newNode->next = adjList[i];
            adjList[i] = newNode;
        }

        free(arr);
    }
}

// Função para calcular e exibir as propriedades do vértice
void calculateVerticeProperties(Node* adjList[], int numVertices, int vertice) {
    int outDegree = 0;
    int inDegree = 0;

    printf("Sucessores do vertice %d: ", vertice);
    Node* temp = adjList[vertice];
    while (temp) {
        outDegree++;
        printf("%d ", temp->vertice);
        temp = temp->next;
    }
    printf("\n");

    printf("Predecessores do vertice %d: ", vertice);
    for (int i = 1; i <= numVertices; i++) {
        temp = adjList[i];
        while (temp) {
            if (temp->vertice == vertice) {
                inDegree++;
                printf("%d ", i);
                break;
            }
            temp = temp->next;
        }
    }
    printf("\n");

    printf("Grau de saida do vertice %d: %d\n", vertice, outDegree);
    printf("Grau de entrada do vertice %d: %d\n", vertice, inDegree);
}
