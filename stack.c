#include "stack.h"

// Função para criar um novo nó da pilha
StackNode* createStackNode(int data) {
    StackNode* stackNode = (StackNode*)malloc(sizeof(StackNode));
    if (!stackNode) {
        printf("Erro ao alocar memória para o nó da pilha.\n");
        exit(1);
    }
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

// Função para verificar se a pilha está vazia
int isEmpty(StackNode* root) {
    return !root;
}

// Função para empurrar um elemento na pilha
void push(StackNode** root, int data) {
    StackNode* stackNode = createStackNode(data);
    stackNode->next = *root;
    *root = stackNode;
    //printf("%d empurrado para a pilha.\n", data);
}

// Função para remover o topo da pilha
int pop(StackNode** root) {
    if (isEmpty(*root)) {
        printf("Erro: Tentativa de remover de uma pilha vazia.\n");
        exit(1);
    }
    StackNode* temp = *root;
    *root = (*root)->next;
    int popped = temp->data;
    free(temp);
    return popped;
}

// Função para pegar o valor do topo da pilha
int peek(StackNode* root) {
    if (isEmpty(root)) {
        printf("A pilha está vazia.\n");
        exit(1);
    }
    return root->data;
}
