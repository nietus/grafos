#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

// Definição do nó para a lista de adjacências
typedef struct Node {
    int vertice;
    struct Node* next;
} Node;

// Funções para manipular o grafo
Node* createNode(int v);
void addEdge(Node* adjList[], int u, int v);
void sortAdjacencyList(Node* adjList[], int numVertices);
void calculateVerticeProperties(Node* adjList[], int numVertices, int vertice);

// Estrutura para armazenar informações dos vértices durante a DFS
typedef struct {
    int discover;
    int finish;
    int parent;
} VertexInfo;

#endif
