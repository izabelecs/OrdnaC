#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

void iniciarLista(Lista *lista)
{
    lista->inicio = NULL;
    lista->quantidade = 0;
}

int inserirLista(Lista *lista, const char *data)
{
    No *novo = malloc(sizeof(No));

    if (novo == NULL)
        return 0;

    strcpy(novo->data, data);
    novo->proximo = NULL;

    if (lista->inicio == NULL ||
        strcmp(data, lista->inicio->data) < 0)
    {
        novo->proximo = lista->inicio;
        lista->inicio = novo;
    }
    else
    {
        No *atual = lista->inicio;

        while (atual->proximo != NULL &&
               strcmp(atual->proximo->data, data) <= 0)
        {
            atual = atual->proximo;
        }

        novo->proximo = atual->proximo;
        atual->proximo = novo;
    }

    lista->quantidade++;
    return 1;
}

int removerLista(Lista *lista, const char *data)
{
    No *atual = lista->inicio;
    No *anterior = NULL;

    while (atual != NULL && strcmp(atual->data, data) < 0)
    {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL || strcmp(atual->data, data) != 0)
        return 0;

    if (anterior == NULL)
        lista->inicio = atual->proximo;
    else
        anterior->proximo = atual->proximo;

    free(atual);
    lista->quantidade--;

    return 1;
}

void listarOrdenadoLista(const Lista *lista)
{
    No *atual = lista->inicio;

    if (atual == NULL)
    {
        printf("A lista esta vazia!\n");
        return;
    }

    printf("\nDatas em ordem cronologica:\n");

    while (atual != NULL)
    {
        printf("%s\n", atual->data);
        atual = atual->proximo;
    }
}

void liberarLista(Lista *lista)
{
    No *atual = lista->inicio;

    while (atual != NULL)
    {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    lista->inicio = NULL;
    lista->quantidade = 0;
}
