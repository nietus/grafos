#include "graph.h"

int main() {
    char filename[100] = "data/graph-test-50000-1.txt";
    int vertice;

    printf("Digite o numero do vertice: ");
    scanf("%d", &vertice);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    int numVertices, numEdges;
    fscanf(file, "%d %d", &numVertices, &numEdges);

    // Criando a lista de adjacências nula
    Node* adjList[numVertices + 1];
    for (int i = 1; i <= numVertices; i++) {
        adjList[i] = NULL;
    }

    // Preenchendo a lista de adjacências
    int u, v;
    for (int i = 0; i < numEdges; i++) {
        fscanf(file, "%d %d", &u, &v);
        addEdge(adjList, u, v);
    }
    fclose(file);

    // Calculando as propriedades do vértice
    calculateverticeProperties(adjList, numVertices, vertice);

    // Liberando a memória alocada
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