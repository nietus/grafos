#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

// Node pra lista
typedef struct Node {
    int vertice;
    struct Node* next;
} Node;

Node* createNode(int v);
void addEdge(Node* adjList[], int u, int v);
void calculateverticeProperties(Node* adjList[], int numVertices, int vertice);

#endif