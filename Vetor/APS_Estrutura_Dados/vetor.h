#ifndef VETOR_H
#define VETOR_H

#define CAPACIDADE 100000

typedef struct
{
    char datas[CAPACIDADE][20];
    int quantidade;

} Vetor;

void iniciarVetor(Vetor *vetor);

int inserirVetor(Vetor *vetor, const char *data);

int removerVetor(Vetor *vetor, const char *data);

void listarOrdenadoVetor(const Vetor *vetor);

void ordenarVetor(const Vetor *vetor);

#endif
