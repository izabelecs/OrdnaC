
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lista.h"
#include "testes.h"

#define MAX_DATAS 100000
#define REPETICOES 10
#define FREQUENCIA_CPU 2900000000.0

static char datas[MAX_DATAS][20];

int carregarDatasLista()
{
    FILE *arquivo = fopen("datas_teste.csv", "r");

    if (arquivo == NULL)
    {
        printf("Erro: datas_teste.csv nao encontrado!\n");
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

static void ordenarResultados(double valores[], int quantidade)
{
    for (int i = 0; i < quantidade - 1; i++)
    {
        for (int j = i + 1; j < quantidade; j++)
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

static void salvarMedidas(FILE *arquivo, const char *operacao,
                          int quantidade, double valores[])
{
    ordenarResultados(valores, REPETICOES);

    double menor = valores[0];
    double maior = valores[REPETICOES - 1];
    double mediana = (valores[4] + valores[5]) / 2.0;

    printf("%s - %d datas: %.6f segundos (mediana)\n",
           operacao, quantidade, mediana);

    fprintf(arquivo, "%s,%d,%.9f,%.9f,%.9f,%.0f,%.0f,%.0f\n",
            operacao, quantidade,
            menor, maior, mediana,
            menor * FREQUENCIA_CPU,
            maior * FREQUENCIA_CPU,
            mediana * FREQUENCIA_CPU);
}

static void percorrerLista(const Lista *lista)
{
    const No *atual = lista->inicio;
    volatile unsigned long quantidadePercorrida = 0;

    while (atual != NULL)
    {
        quantidadePercorrida++;
        atual = atual->proximo;
    }

    (void)quantidadePercorrida;
}

void testarSomenteListagemLista()
{
    // Quantidades de registros utilizadas nos testes
    int tamanhos[5] = {1000, 5000, 10000, 50000, 100000};

    // Quantidade de repeticoes da listagem
    int repeticoes = 100;

    // Carrega as datas do arquivo
    int total = carregarDatasLista();

    if (total != 100000)
    {
        printf("Erro ao carregar as 100000 datas!\n");
        return;
    }

    // Arquivo separado para nao apagar os testes antigos
    FILE *arquivo = fopen("nova_listagem_lista.csv", "w");

    if (arquivo == NULL)
    {
        printf("Erro ao criar arquivo!\n");
        return;
    }

    fprintf(arquivo,
        "Operacao,Quantidade,Menor,Maior,Mediana,CiclosMenor,CiclosMaior,CiclosMediana\n");

    // Testa cada quantidade de registros
    for (int t = 0; t < 5; t++)
    {
        int n = tamanhos[t];

        double tempos[REPETICOES];

        Lista lista;

        iniciarLista(&lista);

        // Prepara a lista antes de medir a listagem
        for (int i = 0; i < n; i++)
        {
            if (inserirLista(&lista, datas[i]) == 0)
            {
                printf("Erro ao inserir dados!\n");
                liberarLista(&lista);
                fclose(arquivo);
                return;
            }
        }

        printf("\nTestando listagem com %d registros...\n", n);

        // Faz dez medicoes independentes
        for (int r = 0; r < REPETICOES; r++)
        {
            clock_t inicio = clock();

            // Percorre a mesma lista 100 vezes
            for (int j = 0; j < repeticoes; j++)
            {
                percorrerLista(&lista);
            }

            clock_t fim = clock();

            // Tempo medio de uma listagem
            tempos[r] =
                ((double)(fim - inicio) / CLOCKS_PER_SEC)
                / repeticoes;

            printf("Teste %d: %.9f segundos\n",
                   r + 1, tempos[r]);
        }

        // Salva menor, maior e mediana
        salvarMedidas(arquivo, "Listagem", n, tempos);

        // Libera a memoria da lista
        liberarLista(&lista);
    }

    fclose(arquivo);

    printf("\nNova listagem salva com sucesso!\n");
}

void testarDesempenhoLista()
{
    int total = carregarDatasLista();

    if (total == 0)
        return;

    int tamanhos[] = {1000, 5000, 10000, 50000, 100000};

    FILE *arquivo = fopen("resultados_lista.csv", "w");

    if (arquivo == NULL)
    {
        printf("Erro ao criar resultados_lista.csv!\n");
        return;
    }

    fprintf(arquivo,
            "Operacao,Quantidade,Menor,Maior,Mediana,CiclosMenor,CiclosMaior,CiclosMediana\n");

    printf("\n===== TESTES DA LISTA ENCADEADA =====\n");

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
            Lista lista;
            iniciarLista(&lista);

            clock_t inicio = clock();

            int sucesso = 1;

            for (int i = 0; i < n; i++)
            {
                if (inserirLista(&lista, datas[i]) == 0)
                {
                    sucesso = 0;
                    break;
                }
            }

            clock_t fim = clock();

            if (sucesso == 0)
            {
                printf("Erro de memoria durante a insercao!\n");
                liberarLista(&lista);
                fclose(arquivo);
                return;
            }

            insercao[r] = (double)(fim - inicio) / CLOCKS_PER_SEC;

            inicio = clock();

            percorrerLista(&lista);

            fim = clock();

            listagem[r] = (double)(fim - inicio) / CLOCKS_PER_SEC;

            inicio = clock();

            for (int i = 0; i < n; i++)
                removerLista(&lista, datas[i]);

            fim = clock();

            remocao[r] = (double)(fim - inicio) / CLOCKS_PER_SEC;

            liberarLista(&lista);
        }

        salvarMedidas(arquivo, "Insercao", n, insercao);
        salvarMedidas(arquivo, "Remocao", n, remocao);
        salvarMedidas(arquivo, "Listagem", n, listagem);

        fflush(arquivo);
    }

    fclose(arquivo);

    printf("\nResultados salvos em resultados_lista.csv!\n");
}

    void calcularCustoPonderadoLista()
{
    FILE *entrada = fopen("resultados_lista.csv", "r");

    if (entrada == NULL)
    {
        printf("Arquivo de resultados nao encontrado!\n");
        return;
    }

    FILE *saida = fopen("custo_ponderado_lista.csv", "w");

    if (saida == NULL)
    {
        printf("Erro ao criar arquivo de custo ponderado!\n");
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
