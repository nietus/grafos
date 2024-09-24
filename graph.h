// graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

// Estrutura para um nó na lista de adjacência
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// Função para adicionar uma aresta ao grafo
void addEdge(Node* adjList[], int u, int v);

// Função para ordenar as listas de adjacência em ordem crescente
void sortAdjacencyList(Node* adjList[], int numVertices);

// Função para liberar a memória da lista de adjacência
void freeAdjacencyList(Node* adjList[], int numVertices);

// Função para liberar uma única lista de adjacência
void freeAdjList(Node* adjList[], int vertex);

// Função para obter os predecessores de um vértice
int* getPredecessors(Node* adjList[], int numVertices, int vertex, int* count);

#endif // GRAPH_H
