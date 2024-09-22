#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

// Definição do nó da pilha
typedef struct StackNode {
    int data;
    struct StackNode* next;
} StackNode;

// Função para criar um novo nó da pilha
StackNode* createStackNode(int data);

// Função para verificar se a pilha está vazia
int isEmpty(StackNode* root);

// Função para empurrar um elemento na pilha
void push(StackNode** root, int data);

// Função para remover o topo da pilha
int pop(StackNode** root);

// Função para pegar o valor do topo da pilha
int peek(StackNode* root);

#endif
