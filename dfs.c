#include "dfs.h"
#include "stack.h"

void DFS_Visit_Iterative(Node *adjList[], int startVertex, EdgeClassification **edgeClassList, VertexInfo info[], int *timeCounter)
{
    StackNode *stack = NULL;
    push(&stack, startVertex); // Empurrando o vértice inicial na pilha

    while (!isEmpty(stack))
    {
        int u = pop(&stack); // Desempilha o próximo vértice a ser processado

        if (info[u].discover == 0)
        {
            info[u].discover = ++(*timeCounter);
            //printf("Visitando o vértice %d (Descoberto: %d)\n", u, info[u].discover);
        }

        Node *temp = adjList[u];
        while (temp)
        {
            int v = temp->vertice;

            if (info[v].discover == 0)
            {
                // Aresta de árvore
                push(&stack, v);
                addClassifiedEdge(edgeClassList, u, v, TREE);
            }
            else if (info[v].finish == 0)
            {
                // Aresta de retorno
                addClassifiedEdge(edgeClassList, u, v, BACK);
            }
            else if (info[u].discover < info[v].discover && info[v].finish != 0)
            {
                // Aresta de avanço (v já foi explorado e u foi descoberto antes de v)
                addClassifiedEdge(edgeClassList, u, v, FORWARD);
            }
            else
            {
                // Aresta cruzada (u e v não fazem parte da mesma árvore de recursão)
                addClassifiedEdge(edgeClassList, u, v, CROSS);
            }

            temp = temp->next;
        }

        info[u].finish = ++(*timeCounter);
        //printf("Finalizando o vértice %d (Finalizado: %d)\n", u, info[u].finish);
    }
}

// Função para classificar e exibir as arestas divergentes do vértice
void classifyOutgoingEdges(Node *adjList[], VertexInfo info[], int numVertices, int vertice, EdgeClassification *edgeClassList)
{
    printf("\nClassificação das arestas divergentes do vértice %d:\n", vertice);
    Node *temp = adjList[vertice];
    if (!temp)
    {
        printf("Nenhuma aresta divergente encontrada para o vértice %d.\n", vertice);
        return;
    }
    while (temp)
    {
        int v = temp->vertice;
        // Encontrar a classificação da aresta (vertice -> v)
        EdgeClassification *current = edgeClassList;
        EdgeType type = CROSS; // Padrão
        while (current)
        {
            if (current->u == vertice && current->v == v)
            {
                type = current->type;
                break;
            }
            current = current->next;
        }
        // Exibir a classificação
        printf("%d -> %d: ", vertice, v);
        switch (type)
        {
        case TREE:
            printf("Aresta de Árvore");
            break;
        case BACK:
            printf("Aresta de Retorno");
            break;
        case FORWARD:
            printf("Aresta de Avanço");
            break;
        case CROSS:
            printf("Aresta de Cruzamento");
            break;
        }
        printf("\n");
        temp = temp->next;
    }
}

// Função para adicionar uma aresta classificada à lista
void addClassifiedEdge(EdgeClassification **list, int u, int v, EdgeType type)
{
    EdgeClassification *newEdge = (EdgeClassification *)malloc(sizeof(EdgeClassification));
    if (!newEdge)
    {
        printf("Erro na alocação de memória para EdgeClassification.\n");
        exit(1);
    }
    newEdge->u = u;
    newEdge->v = v;
    newEdge->type = type;
    newEdge->next = *list;
    *list = newEdge;

    //printf("Aresta adicionada: %d -> %d (Tipo: %d)\n", u, v, type);
}

// Função principal da DFS
void DFS(Node *adjList[], int numVertices, int startVertex, EdgeClassification **edgeClassList, VertexInfo info[], int *timeCounter)
{
    // Inicializar informações dos vértices
    for (int i = 1; i <= numVertices; i++)
    {
        info[i].discover = 0;
        info[i].finish = 0;
        info[i].parent = -1;
    }

    printf("Iniciando DFS a partir do vértice %d\n", startVertex);
    DFS_Visit_Iterative(adjList, startVertex, edgeClassList, info, timeCounter);
    printf("DFS concluída!\n");
}

// Função para exibir todas as classificações de arestas
void displayClassifiedEdges(EdgeClassification *edgeClassList)
{
    int treeCount = 0, backCount = 0, forwardCount = 0, crossCount = 0;

    printf("\nClassificação das Arestas:\n");
    EdgeClassification *temp = edgeClassList;
    while (temp)
    {
        //printf("%d -> %d: ", temp->u, temp->v);
        switch (temp->type)
        {
        case TREE:
            //printf("Aresta de Árvore\n");
            treeCount++;
            break;
        case BACK:
            //printf("Aresta de Retorno\n");
            backCount++;
            break;
        case FORWARD:
            //printf("Aresta de Avanço\n");
            forwardCount++;
            break;
        case CROSS:
            //printf("Aresta Cruzada\n");
            crossCount++;
            break;
        default:
            //printf("Tipo desconhecido\n");
            break;
        }
        temp = temp->next;
    }

    // Exibir o número total de cada tipo de aresta classificada
    printf("\nResumo das Arestas Classificadas:\n");
    printf("Arestas de Árvore: %d\n", treeCount);
    printf("Arestas de Retorno: %d\n", backCount);
    printf("Arestas de Avanço: %d\n", forwardCount);
    printf("Arestas Cruzadas: %d\n", crossCount);
    if (treeCount == 0 && backCount == 0 && forwardCount == 0 && crossCount == 0)
    {
        printf("Nenhuma aresta classificada.\n");
    }
}
