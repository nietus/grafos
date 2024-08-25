#include "graph.h"

// Função para criar um novo nó na lista de adjacências
Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
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

// Função para calcular e exibir o grau de saída, grau de entrada, sucessores e predecessores
void calculateverticeProperties(Node* adjList[], int numVertices, int vertice) {
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
