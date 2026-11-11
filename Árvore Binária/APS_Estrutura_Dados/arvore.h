#ifndef ARVORE_H
#define ARVORE_H

typedef struct No
{
    char data[20];
    struct No *esquerda;
    struct No *direita;
    int quantidade;
} No;

typedef struct
{
    No *raiz;
    int quantidade;
} Arvore;

void iniciarArvore(Arvore *arvore);
int inserirArvore(Arvore *arvore, const char *data);
int removerArvore(Arvore *arvore, const char *data);
void listarOrdenadoArvore(const Arvore *arvore);
void liberarArvore(Arvore *arvore);

#endif
