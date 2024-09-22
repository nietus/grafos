#ifndef DFS_H
#define DFS_H

#include "graph.h"

// Tipos de arestas
typedef enum {
    TREE,
    BACK,
    FORWARD,
    CROSS
} EdgeType;

// Estrutura para armazenar informações das arestas classificadas
typedef struct EdgeClassification {
    int u;
    int v;
    EdgeType type;
    struct EdgeClassification* next;
} EdgeClassification;

// Função para realizar a DFS
void DFS(Node* adjList[], int numVertices, int startVertex, EdgeClassification** edgeClassList, VertexInfo info[], int* timeCounter);

// Função para adicionar uma aresta classificada à lista
void addClassifiedEdge(EdgeClassification** list, int u, int v, EdgeType type);

// Função para exibir as arestas de árvore
void displayTreeEdges(EdgeClassification* edgeClassList);

// Função para exibir todas as classificações de arestas
void displayClassifiedEdges(EdgeClassification* edgeClassList);

// Função para classificar e exibir as arestas divergentes do vértice
void classifyOutgoingEdges(Node* adjList[], VertexInfo info[], int numVertices, int vertice, EdgeClassification* edgeClassList);

#endif
