
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arvore.h"
#include "testes.h"

#define MAX_DATAS 100000
#define REPETICOES 10
#define REPETICOES_LISTAGEM 100
#define FREQUENCIA_CPU 2900000000.0

static char datas[MAX_DATAS][20];
static volatile unsigned long totalPercorrido = 0;

static int carregarDatas()
{
    FILE *arquivo = fopen("datas_teste.csv", "r");

    if (arquivo == NULL)
    {
        printf("Arquivo datas_teste.csv nao encontrado!\n");
        return 0;
    }

    char linha[100];
    int quantidade = 0;

    while (quantidade < MAX_DATAS &&
           fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        linha[strcspn(linha, "\r\n")] = '\0';

        if (strlen(linha) == 19)
        {
            strcpy(datas[quantidade], linha);
            quantidade++;
        }
    }

    fclose(arquivo);
    return quantidade;
}

static void percorrerNos(const No *no)
{
    if (no == NULL)
        return;

    percorrerNos(no->esquerda);

    totalPercorrido += (unsigned long)no->quantidade;

    percorrerNos(no->direita);
}

static void ordenarResultados(double valores[])
{
    for (int i = 0; i < REPETICOES - 1; i++)
    {
        for (int j = i + 1; j < REPETICOES; j++)
        {
            if (valores[j] < valores[i])
            {
                double auxiliar = valores[i];
                valores[i] = valores[j];
                valores[j] = auxiliar;
            }
        }
    }
}

static double salvarMedidas(FILE *arquivo,
                           const char *operacao,
                           int quantidade,
                           double valores[])
{
    ordenarResultados(valores);

    double menor = valores[0];
    double maior = valores[REPETICOES - 1];
    double mediana = (valores[4] + valores[5]) / 2.0;

    fprintf(arquivo,
            "%s,%d,%.9f,%.9f,%.9f,%.0f,%.0f,%.0f\n",
            operacao, quantidade,
            menor, maior, mediana,
            menor * FREQUENCIA_CPU,
            maior * FREQUENCIA_CPU,
            mediana * FREQUENCIA_CPU);

    printf("%s - %d datas: %.6f segundos\n",
           operacao, quantidade, mediana);

    return mediana * FREQUENCIA_CPU;
}

void testarDesempenhoArvore()
{
    int total = carregarDatas();

    if (total == 0)
        return;

    int tamanhos[] = {1000, 5000, 10000, 50000, 100000};

    FILE *arquivo = fopen("resultados_arvore.csv", "w");

    if (arquivo == NULL)
    {
        printf("Erro ao criar resultados_arvore.csv!\n");
        return;
    }

    fprintf(arquivo,
            "Operacao,Quantidade,Menor,Maior,Mediana,CiclosMenor,CiclosMaior,CiclosMediana\n");

    printf("\n===== TESTES DA ARVORE =====\n");

    for (int t = 0; t < 5; t++)
    {
        int n = tamanhos[t];

        if (n > total)
            continue;

        double insercao[REPETICOES];
        double remocao[REPETICOES];
        double listagem[REPETICOES];

        printf("\nTestando %d datas...\n", n);

        for (int r = 0; r < REPETICOES; r++)
        {
            Arvore arvore;
            iniciarArvore(&arvore);

            clock_t inicio = clock();

            int sucesso = 1;

            for (int i = 0; i < n; i++)
            {
                if (inserirArvore(&arvore, datas[i]) == 0)
                {
                    sucesso = 0;
                    break;
                }
            }

            clock_t fim = clock();

            if (sucesso == 0)
            {
                printf("Erro de memoria na insercao!\n");
                liberarArvore(&arvore);
                fclose(arquivo);
                return;
            }

            insercao[r] = (double)(fim - inicio) / CLOCKS_PER_SEC;

            totalPercorrido = 0;

            inicio = clock();

            for (int i = 0; i < REPETICOES_LISTAGEM; i++)
                percorrerNos(arvore.raiz);

            fim = clock();

            listagem[r] = ((double)(fim - inicio) / CLOCKS_PER_SEC)
                          / REPETICOES_LISTAGEM;

            if (totalPercorrido !=
                (unsigned long)n * REPETICOES_LISTAGEM)
            {
                printf("Erro na contagem da listagem!\n");
                liberarArvore(&arvore);
                fclose(arquivo);
                return;
            }

            inicio = clock();

            for (int i = 0; i < n; i++)
            {
                if (removerArvore(&arvore, datas[i]) == 0)
                {
                    sucesso = 0;
                    break;
                }
            }

            fim = clock();

            if (sucesso == 0 || arvore.quantidade != 0)
            {
                printf("Erro durante a remocao!\n");
                liberarArvore(&arvore);
                fclose(arquivo);
                return;
            }

            remocao[r] = (double)(fim - inicio) / CLOCKS_PER_SEC;

            liberarArvore(&arvore);
        }

        salvarMedidas(arquivo, "Insercao", n, insercao);
        salvarMedidas(arquivo, "Remocao", n, remocao);
        salvarMedidas(arquivo, "Listagem", n, listagem);

        fflush(arquivo);
    }

    fclose(arquivo);

    printf("\nResultados salvos em resultados_arvore.csv!\n");
}

void calcularCustoPonderadoArvore()
{
    FILE *entrada = fopen("resultados_arvore.csv", "r");

    if (entrada == NULL)
    {
        printf("Arquivo de resultados nao encontrado!\n");
        return;
    }

    FILE *saida = fopen("custo_ponderado_arvore.csv", "w");

    if (saida == NULL)
    {
        printf("Erro ao criar arquivo de custo!\n");
        fclose(entrada);
        return;
    }

    char linha[300];
    char operacao[30];
    int quantidade;

    double menor, maior, mediana;
    double ciclosMenor, ciclosMaior, ciclosMediana;

    int tamanhos[] = {1000, 5000, 10000, 50000, 100000};

    double insercao[5] = {0};
    double remocao[5] = {0};
    double listagem[5] = {0};

    fgets(linha, sizeof(linha), entrada);

    while (fgets(linha, sizeof(linha), entrada) != NULL)
    {
        int campos = sscanf(linha,
            "%29[^,],%d,%lf,%lf,%lf,%lf,%lf,%lf",
            operacao, &quantidade,
            &menor, &maior, &mediana,
            &ciclosMenor, &ciclosMaior, &ciclosMediana);

        if (campos != 8)
            continue;

        for (int i = 0; i < 5; i++)
        {
            if (quantidade == tamanhos[i])
            {
                if (strcmp(operacao, "Insercao") == 0)
                    insercao[i] = ciclosMediana;

                if (strcmp(operacao, "Remocao") == 0)
                    remocao[i] = ciclosMediana;

                if (strcmp(operacao, "Listagem") == 0)
                    listagem[i] = ciclosMediana;
            }
        }
    }

    fprintf(saida, "Quantidade,CustoPonderado\n");

    for (int i = 0; i < 5; i++)
    {
        double custo = 0.70 * insercao[i]
                     + 0.15 * remocao[i]
                     + 0.15 * listagem[i];

        fprintf(saida, "%d,%.0f\n", tamanhos[i], custo);

        printf("%d datas: %.0f ciclos\n", tamanhos[i], custo);
    }

    fclose(entrada);
    fclose(saida);

    printf("\nCusto ponderado salvo com sucesso!\n");
}
