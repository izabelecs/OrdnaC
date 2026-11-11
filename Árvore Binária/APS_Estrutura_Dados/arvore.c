#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

void iniciarArvore(Arvore *arvore)
{
    arvore->raiz = NULL;
    arvore->quantidade = 0;
}

int inserirArvore(Arvore *arvore, const char *data)
{
    No **atual = &arvore->raiz;

    while (*atual != NULL)
    {
        int comparacao = strcmp(data, (*atual)->data);

        if (comparacao == 0)
        {
            (*atual)->quantidade++;
            arvore->quantidade++;
            return 1;
        }

        if (comparacao < 0)
            atual = &(*atual)->esquerda;
        else
            atual = &(*atual)->direita;
    }

    No *novo = malloc(sizeof(No));

    if (novo == NULL)
        return 0;

    strcpy(novo->data, data);
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->quantidade = 1;

    *atual = novo;

    arvore->quantidade++;

    return 1;
}

static No *removerNo(No *raiz, const char *data, int *removeu)
{
    if (raiz == NULL)
        return NULL;

    int comparacao = strcmp(data, raiz->data);

    if (comparacao < 0)
    {
        raiz->esquerda = removerNo(raiz->esquerda, data, removeu);
    }
    else if (comparacao > 0)
    {
        raiz->direita = removerNo(raiz->direita, data, removeu);
    }
    else
    {
        *removeu = 1;

        if (raiz->quantidade > 1)
        {
            raiz->quantidade--;
            return raiz;
        }

        if (raiz->esquerda == NULL)
        {
            No *direita = raiz->direita;
            free(raiz);
            return direita;
        }

        if (raiz->direita == NULL)
        {
            No *esquerda = raiz->esquerda;
            free(raiz);
            return esquerda;
        }

        No *sucessor = raiz->direita;

        while (sucessor->esquerda != NULL)
            sucessor = sucessor->esquerda;

        strcpy(raiz->data, sucessor->data);
        raiz->quantidade = sucessor->quantidade;

        No *pai = raiz;
        No *atual = raiz->direita;

        while (atual->esquerda != NULL)
        {
            pai = atual;
            atual = atual->esquerda;
        }

        if (pai == raiz)
            pai->direita = atual->direita;
        else
            pai->esquerda = atual->direita;

        free(atual);
    }

    return raiz;
}

int removerArvore(Arvore *arvore, const char *data)
{
    int removeu = 0;

    arvore->raiz = removerNo(arvore->raiz, data, &removeu);

    if (removeu == 1)
        arvore->quantidade--;

    return removeu;
}

static void mostrarEmOrdem(const No *no)
{
    if (no == NULL)
        return;

    mostrarEmOrdem(no->esquerda);

    for (int i = 0; i < no->quantidade; i++)
        printf("%s\n", no->data);

    mostrarEmOrdem(no->direita);
}

void listarOrdenadoArvore(const Arvore *arvore)
{
    if (arvore->raiz == NULL)
    {
        printf("A arvore esta vazia!\n");
        return;
    }

    printf("\nDatas em ordem cronologica:\n");
    mostrarEmOrdem(arvore->raiz);
}

static void liberarNos(No *no)
{
    if (no == NULL)
        return;

    liberarNos(no->esquerda);
    liberarNos(no->direita);

    free(no);
}

void liberarArvore(Arvore *arvore)
{
    liberarNos(arvore->raiz);

    arvore->raiz = NULL;
    arvore->quantidade = 0;
}
