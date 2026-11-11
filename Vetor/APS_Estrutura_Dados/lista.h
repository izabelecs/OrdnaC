#ifndef LISTA_H
#define LISTA_H

typedef struct No
{
    char data[20];
    struct No *proximo;
} No;

typedef struct
{
    No *inicio;
    int quantidade;
} Lista;

void iniciarLista(Lista *lista);
int inserirLista(Lista *lista, const char *data);
int removerLista(Lista *lista, const char *data);
void listarOrdenadoLista(const Lista *lista);
void liberarLista(Lista *lista);

#endif
