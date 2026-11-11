#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vetor.h"

int validarData(const char *data)
{
    int ano, mes, dia, hora, minuto, segundo;

    if (data == NULL)
        return 0;

    if (strlen(data) != 19)
        return 0;

    if (data[4] != '-' || data[7] != '-' ||
        data[10] != ' ' || data[13] != ':' ||
        data[16] != ':')
        return 0;

    for (int i = 0; i < 19; i++)
    {
        if (i != 4 && i != 7 && i != 10 &&
            i != 13 && i != 16)
        {
            if (data[i] < '0' || data[i] > '9')
                return 0;
        }
    }

    sscanf(data, "%d-%d-%d %d:%d:%d",
           &ano, &mes, &dia, &hora, &minuto, &segundo);

    if (mes < 1 || mes > 12)
        return 0;

    if (dia < 1 || dia > 31)
        return 0;

    if (hora < 0 || hora > 23)
        return 0;

    if (minuto < 0 || minuto > 59)
        return 0;

    if (segundo < 0 || segundo > 59)
        return 0;

    return 1;
}

void iniciarVetor(Vetor *vetor)
{
    vetor->quantidade = 0;
}

int inserirVetor(Vetor *vetor, const char *data)
{
    if (vetor->quantidade == CAPACIDADE)
    {
        return 0;
    }

    if (validarData(data) == 0)
    {
        return 0;
    }

    strcpy(vetor->datas[vetor->quantidade], data);

    vetor->quantidade++;

    return 1;
}

int removerVetor(Vetor *vetor, const char *data)
{
    for (int i = 0; i < vetor->quantidade; i++)
    {
        if (strcmp(vetor->datas[i], data) == 0)
        {
            for (int j = i; j < vetor->quantidade - 1; j++)
            {
                strcpy(vetor->datas[j], vetor->datas[j + 1]);
            }

            vetor->quantidade--;

            return 1;
        }
    }

    return 0;
}

int compararDatas(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

void ordenarVetor(const Vetor *vetor)
{
    if (vetor->quantidade == 0)
    {
        return;
    }

    char (*copia)[20] = malloc(vetor->quantidade * sizeof(copia[0]));

    if (copia == NULL)
    {
        return;
    }

    for (int i = 0; i < vetor->quantidade; i++)
    {
        strcpy(copia[i], vetor->datas[i]);
    }

    qsort(copia, vetor->quantidade, sizeof(copia[0]), compararDatas);

    free(copia);
}

void listarOrdenadoVetor(const Vetor *vetor)
{
    if (vetor->quantidade == 0)
    {
        printf("O vetor esta vazio!\n");
        return;
    }

    char (*copia)[20] = malloc(vetor->quantidade * sizeof(copia[0]));

    if (copia == NULL)
    {
        printf("Erro ao organizar as datas!\n");
        return;
    }

    for (int i = 0; i < vetor->quantidade; i++)
    {
        strcpy(copia[i], vetor->datas[i]);
    }

    qsort(copia, vetor->quantidade, sizeof(copia[0]), compararDatas);

    printf("\nDatas em ordem cronologica:\n");

    for (int i = 0; i < vetor->quantidade; i++)
    {
        printf("%s\n", copia[i]);
    }

    free(copia);
}
