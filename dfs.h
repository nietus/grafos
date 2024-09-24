// dfs.h
#ifndef DFS_H
#define DFS_H

#include "graph.h"

// Cores para DFS
typedef enum { WHITE, GRAY, BLACK } Color;

// Estrutura para armazenar informações de cada vértice
typedef struct VertexInfo {
    Color color;
    int discoveryTime;
    int finishTime;
    int parent;
} VertexInfo;

// Estrutura para classificar as arestas
typedef struct EdgeClassification {
    int u;
    int v;
    char type[20];
    struct EdgeClassification* next;
} EdgeClassification;

// Função para realizar a DFS
void DFS(Node* adjList[], int numVertices, int startVertex, 
         EdgeClassification** edgeClassList, VertexInfo info[], 
         int* timeCounter, int traversal[], int* traversalIndex);

// Função para classificar as arestas divergentes de um vértice específico
void classifyOutgoingEdges(Node* adjList[], VertexInfo info[], 
                           int vertice, EdgeClassification** edgeClassList);

// Função para exibir as arestas classificadas
void displayClassifiedEdges(EdgeClassification* edgeClassList);

#endif // DFS_H
