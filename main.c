// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "dfs.h"

int main() {
    char* filename = "data/graph-test-100-1.txt";
    int vertice = 1;

    // Abrindo o arquivo
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    int numVertices, numEdges;
    if (fscanf(file, "%d %d", &numVertices, &numEdges) != 2) {
        printf("Erro na leitura do número de vértices e arestas.\n");
        fclose(file);
        return 1;
    }

    if (vertice < 1 || vertice > numVertices) {
        printf("Vértice inválido. Deve estar entre 1 e %d.\n", numVertices);
        fclose(file);
        return 1;
    }

    // Criando a lista de adjacências nula
    Node* adjList[numVertices + 1];
    for (int i = 1; i <= numVertices; i++) {
        adjList[i] = NULL;
    }

    // Preenchendo a lista de adjacências
    int u, v;
    for (int i = 0; i < numEdges; i++) {
        if (fscanf(file, "%d %d", &u, &v) != 2) {
            printf("Erro na leitura da aresta %d.\n", i + 1);
            fclose(file);
            freeAdjacencyList(adjList, numVertices);
            return 1;
        }
        if (u < 1 || u > numVertices || v < 1 || v > numVertices) {
            printf("Aresta inválida: %d-->%d.\n", u, v);
            fclose(file);
            freeAdjacencyList(adjList, numVertices);
            return 1;
        }
        addEdge(adjList, u, v);
    }
    fclose(file);

    // Ordenar as listas de adjacência em ordem lexicográfica
    sortAdjacencyList(adjList, numVertices);

    // Realizando a DFS a partir do vértice especificado
    EdgeClassification* edgeClassList = NULL;
    VertexInfo* info = (VertexInfo*)malloc((numVertices + 1) * sizeof(VertexInfo));
    if (!info) {
        printf("Erro na alocação de memória para VertexInfo.\n");
        freeAdjacencyList(adjList, numVertices);
        return 1;
    }
    int timeCounter = 0;

    // Inicializa a lista de caminhada da DFS
    int* traversal = (int*)malloc((numVertices + 1) * sizeof(int));
    if (!traversal) {
        printf("Erro na alocação de memória para a caminhada da DFS.\n");
        free(info);
        freeAdjacencyList(adjList, numVertices);
        return 1;
    }
    int traversalIndex = 0;

    DFS(adjList, numVertices, vertice, &edgeClassList, info, &timeCounter, traversal, &traversalIndex);

    // Exibindo as arestas classificadas
    printf("\n");
    displayClassifiedEdges(edgeClassList);

    // Exibindo a caminhada da DFS
    printf("\nDFS Traversal: [");
    for (int i = 0; i < traversalIndex; i++) {
        printf("%d", traversal[i]);
        if (i != traversalIndex - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    // Listando os sucessores do vértice escolhido
    printf("\nSucessors do vértice %d:\n", vertice);
    Node* succ = adjList[vertice];
    if (succ == NULL) {
        printf("Nenhum sucessor.\n");
    } else {
        while (succ) {
            printf("%d ", succ->vertex);
            succ = succ->next;
        }
        printf("\n");
    }

    // Listando os predecessores do vértice escolhido
    int predCount = 0;
    int* predecessors = getPredecessors(adjList, numVertices, vertice, &predCount);
    printf("\nPredecessors do vértice %d:\n", vertice);
    if (predCount == 0) {
        printf("Nenhum predecessor.\n");
    } else {
        for (int i = 0; i < predCount; i++) {
            printf("%d ", predecessors[i]);
        }
        printf("\n");
        free(predecessors);
    }

    // Liberando a memória alocada para as arestas classificadas
    while (edgeClassList) {
        EdgeClassification* tempEdge = edgeClassList;
        edgeClassList = edgeClassList->next;
        free(tempEdge);
    }

    // Liberando a memória alocada para a caminhada da DFS
    free(traversal);

    // Liberando a memória alocada para VertexInfo
    free(info);

    // Liberando a memória alocada para a lista de adjacências
    freeAdjacencyList(adjList, numVertices);

    return 0;
}
