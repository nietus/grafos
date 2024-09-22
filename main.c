#include "graph.h"
#include "dfs.h"

int main() {
    int vertice = 127;
    
    char* filename = "data/graph-test-50000-1.txt";

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
            return 1;
        }
        if (u < 1 || u > numVertices || v < 1 || v > numVertices) {
            printf("Aresta inválida: %d -> %d.\n", u, v);
            fclose(file);
            return 1;
        }
        addEdge(adjList, u, v);
    }
    fclose(file);


    // Realizando a DFS a partir do vértice especificado
    EdgeClassification* edgeClassList = NULL;
    VertexInfo* info = (VertexInfo*)malloc((numVertices + 1) * sizeof(VertexInfo));
    if (!info) {
        printf("Erro na alocação de memória para VertexInfo.\n");
        return 1;
    }
    int timeCounter = 0;

    DFS(adjList, numVertices, vertice, &edgeClassList, info, &timeCounter);

    // Ordenar as listas de adjacência em ordem lexicográfica
    sortAdjacencyList(adjList, numVertices);

    // Calculando as propriedades do vértice
    calculateVerticeProperties(adjList, numVertices, vertice);

    // Classificando e exibindo as arestas divergentes do vértice
    classifyOutgoingEdges(adjList, info, numVertices, vertice, edgeClassList);

    // Exibindo as estatísticas das arestas
    displayClassifiedEdges(edgeClassList);

    // Liberando a memória alocada para a lista de arestas classificadas
    EdgeClassification* tempEdge;
    while (edgeClassList) {
        tempEdge = edgeClassList;
        edgeClassList = edgeClassList->next;
        free(tempEdge);
    }

    free(info);

    // Liberando a memória alocada para a lista de adjacências
    for (int i = 1; i <= numVertices; i++) {
        Node* temp = adjList[i];
        while (temp) {
            Node* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }

    return 0;
}
