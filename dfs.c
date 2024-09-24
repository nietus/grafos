// dfs.c
#include "dfs.h"
#include <string.h>

// Função para adicionar uma aresta classificada à lista (append)
void addClassifiedEdge(EdgeClassification** edgeClassList, int u, int v, const char* type) {
    EdgeClassification* newEdge = (EdgeClassification*)malloc(sizeof(EdgeClassification));
    if (!newEdge) {
        printf("Erro na alocação de memória para EdgeClassification.\n");
        exit(EXIT_FAILURE);
    }
    newEdge->u = u;
    newEdge->v = v;
    strncpy(newEdge->type, type, sizeof(newEdge->type));
    newEdge->type[sizeof(newEdge->type) - 1] = '\0';
    newEdge->next = NULL;

    if (*edgeClassList == NULL) {
        *edgeClassList = newEdge;
    } else {
        EdgeClassification* temp = *edgeClassList;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newEdge;
    }
}

// Função recursiva auxiliar para DFS
void DFSVisit(Node* adjList[], int u, VertexInfo info[], int* timeCounter, 
              EdgeClassification** edgeClassList, int traversal[], int* traversalIndex) {
    info[u].color = GRAY;
    info[u].discoveryTime = ++(*timeCounter);
    traversal[(*traversalIndex)++] = u; // Adiciona o vértice à caminhada

    Node* temp = adjList[u];
    while (temp) {
        int v = temp->vertex;
        if (info[v].color == WHITE) {
            info[v].parent = u;
            // Aresta de árvore
            addClassifiedEdge(edgeClassList, u, v, "Tree Edge");
            DFSVisit(adjList, v, info, timeCounter, edgeClassList, traversal, traversalIndex);
        } else if (info[v].color == GRAY) {
            // Aresta de retorno
            addClassifiedEdge(edgeClassList, u, v, "Back Edge");
        } else if (info[v].discoveryTime > info[u].discoveryTime && 
                   info[v].finishTime < info[u].finishTime) {
            // Aresta para frente
            addClassifiedEdge(edgeClassList, u, v, "Forward Edge");
        } else {
            // Aresta cruzada
            addClassifiedEdge(edgeClassList, u, v, "Cross Edge");
        }
        temp = temp->next;
    }

    info[u].color = BLACK;
    info[u].finishTime = ++(*timeCounter);
}

// Função para realizar a DFS
void DFS(Node* adjList[], int numVertices, int startVertex, 
         EdgeClassification** edgeClassList, VertexInfo info[], 
         int* timeCounter, int traversal[], int* traversalIndex) {
    // Inicializa as informações dos vértices
    for (int i = 1; i <= numVertices; i++) {
        info[i].color = WHITE;
        info[i].discoveryTime = 0;
        info[i].finishTime = 0;
        info[i].parent = -1;
    }

    // Inicia a DFS a partir do vértice especificado
    DFSVisit(adjList, startVertex, info, timeCounter, edgeClassList, traversal, traversalIndex);

    // Continua a DFS para quaisquer vértices não visitados, em ordem crescente
    for (int i = 1; i <= numVertices; i++) {
        if (info[i].color == WHITE) {
            DFSVisit(adjList, i, info, timeCounter, edgeClassList, traversal, traversalIndex);
        }
    }
}

// Função para classificar as arestas divergentes de um vértice específico
void classifyOutgoingEdges(Node* adjList[], VertexInfo info[], 
                           int vertice, EdgeClassification** edgeClassList) {
    Node* temp = adjList[vertice];
    while (temp) {
        int v = temp->vertex;
        if (info[v].parent == vertice) {
            addClassifiedEdge(edgeClassList, vertice, v, "Tree Edge");
        } else if (info[v].color == GRAY) {
            addClassifiedEdge(edgeClassList, vertice, v, "Back Edge");
        } else if (info[v].discoveryTime > info[vertice].discoveryTime && 
                   info[v].finishTime < info[vertice].finishTime) {
            addClassifiedEdge(edgeClassList, vertice, v, "Forward Edge");
        } else {
            addClassifiedEdge(edgeClassList, vertice, v, "Cross Edge");
        }
        temp = temp->next;
    }
}

// Função para exibir as arestas classificadas
void displayClassifiedEdges(EdgeClassification* edgeClassList) {
    EdgeClassification* temp = edgeClassList;
    while (temp) {
        printf("%s: %d-->%d\n", temp->type, temp->u, temp->v);
        temp = temp->next;
    }
}
