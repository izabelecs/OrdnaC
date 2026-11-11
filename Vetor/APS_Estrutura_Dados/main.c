#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vetor.h"

void gerarDatas(Vetor *vetor, int quantidade)
{
    char data[20];

    for (int i = 0; i < quantidade; i++)
    {
        int mes = rand() % 12 + 1;
        int dia = rand() % 28 + 1;
        int hora = rand() % 24;
        int minuto = rand() % 60;
        int segundo = rand() % 60;

        sprintf(data, "2026-%02d-%02d %02d:%02d:%02d",
                mes, dia, hora, minuto, segundo);

        inserirVetor(vetor, data);
    }
}

void prepararDatas(char datas[][20], int quantidade)
{
    for (int i = 0; i < quantidade; i++)
    {
        int mes = rand() % 12 + 1;
        int dia = rand() % 28 + 1;
        int hora = rand() % 24;
        int minuto = rand() % 60;
        int segundo = rand() % 60;

        sprintf(datas[i], "2026-%02d-%02d %02d:%02d:%02d",
                mes, dia, hora, minuto, segundo);
    }
}

void salvarDatas()
{
    static char datas[100000][20];

    FILE *arquivo = fopen("datas_teste.csv", "w");

    if (arquivo == NULL)
    {
        printf("Erro ao criar arquivo!\n");
        return;
    }

    srand(123);

    prepararDatas(datas, 100000);

    for (int i = 0; i < 100000; i++)
    {
        fprintf(arquivo, "%s\n", datas[i]);
    }

    fclose(arquivo);

    printf("Arquivo de datas criado com sucesso!\n");
}

int carregarDatas(char datas[][20])
{
    FILE *arquivo = fopen("datas_teste.csv", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de datas!\n");
        return 0;
    }

    int quantidade = 0;
    char linha[100];

    while (quantidade < 100000 &&
           fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        linha[strcspn(linha, "\n")] = '\0';

        if (strlen(linha) == 19)
        {
            strcpy(datas[quantidade], linha);
            quantidade++;
        }
    }

    fclose(arquivo);

    return quantidade;
}

void criarArquivoCSV()
{
    FILE *arquivo = fopen("resultados_vetor.csv", "r");

    if (arquivo != NULL)
    {
        fclose(arquivo);
        return;
    }

    arquivo = fopen("resultados_vetor.csv", "w");

    if (arquivo == NULL)
    {
        printf("Erro ao criar o arquivo!\n");
        return;
    }

    fprintf(arquivo, "Operacao,Quantidade,Menor,Maior,Mediana,CiclosMenor,CiclosMaior,CiclosMediana\n");

    fclose(arquivo);
}

void salvarResultado(const char *operacao, int quantidade,
                    double menor, double maior, double mediana)
{
    FILE *arquivo = fopen("resultados_vetor.csv", "a");

    if (arquivo == NULL)
    {
        printf("Erro ao salvar o arquivo!\n");
        return;
    }

    double frequencia = 2900000000.0;

    fprintf(arquivo, "%s,%d,%.6f,%.6f,%.6f,%.0f,%.0f,%.0f\n",
            operacao, quantidade, menor, maior, mediana,
            menor * frequencia, maior * frequencia,
            mediana * frequencia);

    fclose(arquivo);
}

void mostrarResultados(double tempos[10], const char *operacao, int quantidade)
{
    double auxiliar;

    for (int i = 0; i < 10; i++)
    {
        for (int j = i + 1; j < 10; j++)
        {
            if (tempos[i] > tempos[j])
            {
                auxiliar = tempos[i];
                tempos[i] = tempos[j];
                tempos[j] = auxiliar;
            }
        }
    }

    double menor = tempos[0];
    double maior = tempos[9];
    double mediana = (tempos[4] + tempos[5]) / 2;

    double frequencia = 2900000000.0;

    double ciclosMenor = menor * frequencia;
    double ciclosMaior = maior * frequencia;
    double ciclosMediana = mediana * frequencia;

    printf("\nMenor tempo: %.6f segundos\n", menor);
    printf("Maior tempo: %.6f segundos\n", maior);
    printf("Mediana: %.6f segundos\n", mediana);
    printf("\nMenor ciclos: %.0f\n", ciclosMenor);
    printf("Maior ciclos: %.0f\n", ciclosMaior);
    printf("Mediana ciclos: %.0f\n", ciclosMediana);

    salvarResultado(operacao, quantidade, menor, maior, mediana);
}

void testarDesempenho(Vetor *vetor)
{
    // Quantidades de registros utilizadas nos testes
    int tamanhos[5] = {1000, 5000, 10000, 50000, 100000};

    // Variaveis utilizadas para medir o tempo
    clock_t inicio, fim;
    double tempo;
    double tempos[10];

    // Quantidade de repeticoes internas
    int repeticoes = 100;

    // Armazena as datas carregadas do CSV
    static char datas[100000][20];

    int total = carregarDatas(datas);

    // Verifica se todas as datas foram carregadas
    if (total != 100000)
    {
        printf("Erro ao carregar as 100000 datas!\n");
        return;
    }

    // Percorre os cinco tamanhos de teste
    for (int i = 0; i < 5; i++)
    {
        printf("\n===== TESTE COM %d REGISTROS =====\n", tamanhos[i]);

        // Limpa o vetor antes de inserir as datas
        iniciarVetor(vetor);

        // Insere a quantidade de datas correspondente ao teste
        for (int k = 0; k < tamanhos[i]; k++)
        {
            inserirVetor(vetor, datas[k]);
        }

        // Realiza dez testes independentes
        for (int j = 0; j < 10; j++)
        {
            inicio = clock();

            // Repete a ordenacao para melhorar a precisao
            for (int r = 0; r < repeticoes; r++)
            {
                ordenarVetor(vetor);
            }

            fim = clock();

            // Calcula o tempo medio de uma ordenacao
            tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC)
                    / repeticoes;

            tempos[j] = tempo;

            printf("Teste %d: %.9f segundos\n", j + 1, tempo);
        }

        // Calcula menor, maior e mediana
        mostrarResultados(tempos, "Ordenacao", tamanhos[i]);
    }
}

void testarInsercao(Vetor *vetor)
{
    int tamanhos[5] = {1000, 5000, 10000, 50000, 100000};

    static char datas[100000][20];

    int total = carregarDatas(datas);

    if (total != 100000)
    {

        printf("Erro ao carregar as 100000 datas!\n");
        return;
    }

    double tempos[10];
    clock_t inicio, fim;

    for (int i = 0; i < 5; i++)
    {

        printf("\n===== INSERCAO COM %d REGISTROS =====\n", tamanhos[i]);

        for (int j = 0; j < 10; j++)
        {
            iniciarVetor(vetor);

            inicio = clock();

            for (int k = 0; k < tamanhos[i]; k++)
            {
                inserirVetor(vetor, datas[k]);
            }

            fim = clock();

            tempos[j] = (double)(fim - inicio) / CLOCKS_PER_SEC;

            printf("Teste %d: %.6f segundos\n", j + 1, tempos[j]);
        }

        mostrarResultados(tempos, "Insercao", tamanhos[i]);
    }
}

void testarRemocao(Vetor *vetor)
{
    int tamanhos[5] = {1000, 5000, 10000, 50000, 100000};

    static char datas[100000][20];

    int total = carregarDatas(datas);

        if (total != 100000)
        {
            printf("Erro ao carregar as 100000 datas!\n");
            return;
        }

    double tempos[10];
    clock_t inicio, fim;

    for (int i = 0; i < 5; i++)
    {

        printf("\n===== REMOCAO COM %d REGISTROS =====\n", tamanhos[i]);

        for (int j = 0; j < 10; j++)
        {
            iniciarVetor(vetor);

            for (int k = 0; k < tamanhos[i]; k++)
            {
                inserirVetor(vetor, datas[k]);
            }

            inicio = clock();

            for (int k = 0; k < tamanhos[i]; k++)
            {
                removerVetor(vetor, datas[k]);
            }

            fim = clock();

            tempos[j] = (double)(fim - inicio) / CLOCKS_PER_SEC;

            printf("Teste %d: %.6f segundos\n", j + 1, tempos[j]);
        }

        mostrarResultados(tempos, "Remocao", tamanhos[i]);
    }
}

void calcularCustoPonderado()
{
    FILE *arquivo = fopen("resultados_vetor.csv", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o CSV!\n");
        return;
    }

    int tamanhos[5] = {1000, 5000, 10000, 50000, 100000};

    double insercao[5] = {0};
    double remocao[5] = {0};
    double ordenacao[5] = {0};

    char operacao[30];
    char linha[200];

    int quantidade;
    double menor, maior, mediana;
    double ciclosMenor, ciclosMaior, ciclosMediana;

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        if (sscanf(linha, "%29[^,],%d,%lf,%lf,%lf,%lf,%lf,%lf",
                   operacao, &quantidade, &menor, &maior,
                   &mediana, &ciclosMenor, &ciclosMaior,
                   &ciclosMediana) == 8)
        {
            for (int i = 0; i < 5; i++)
            {
                if (quantidade == tamanhos[i])
                {
                    if (strcmp(operacao, "Insercao") == 0)
                        insercao[i] = ciclosMediana;

                    if (strcmp(operacao, "Remocao") == 0)
                        remocao[i] = ciclosMediana;

                    if (strcmp(operacao, "Ordenacao") == 0)
                        ordenacao[i] = ciclosMediana;
                }
            }
        }
    }

    fclose(arquivo);

    FILE *saida = fopen("custo_ponderado_vetor.csv", "w");

    if (saida == NULL)
    {
        printf("Erro ao criar arquivo de custos!\n");
        return;
    }

    fprintf(saida, "Quantidade,CustoPonderado\n");

    for (int i = 0; i < 5; i++)
    {
        double custo = 0.70 * insercao[i]
                     + 0.15 * remocao[i]
                     + 0.15 * ordenacao[i];

        printf("%d registros: %.0f ciclos\n", tamanhos[i], custo);

        fprintf(saida, "%d,%.0f\n", tamanhos[i], custo);
    }

    fclose(saida);

    printf("Arquivo de custos criado com sucesso!\n");
}

int main()
{
    static Vetor vetor;

    iniciarVetor(&vetor);
    criarArquivoCSV();

    int opcao;
    char data[100];

    do
    {
        printf("\n===== VETOR NAO ORDENADO =====\n");
        printf("1 - Inserir data\n");
        printf("2 - Remover data\n");
        printf("3 - Listar datas ordenadas\n");
        printf("4 - Mostrar quantidade\n");
        printf("5 - Gerar 1000 datas automaticamente\n");
        printf("6 - Testar desempenho do vetor\n");
        printf("7 - Testar insercao do vetor\n");
        printf("8 - Testar remocao do vetor\n");
        printf("9 - Criar arquivo de datas para a APS\n");
        printf("10 - Testar leitura do arquivo CSV\n");
        printf("11 - Calcular custo ponderado\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            opcao = -1;
        }

        int caractere;
        while ((caractere = getchar()) != '\n' &&
               caractere != EOF) {}

        switch (opcao)
        {
            case 1:
                printf("Digite a data (AAAA-MM-DD HH:MM:SS): ");
                if (fgets(data, sizeof(data), stdin) == NULL)
                    break;

                data[strcspn(data, "\n")] = '\0';

                if (inserirVetor(&vetor, data) == 1)
                    printf("Data inserida com sucesso!\n");
                else
                    printf("Nao foi possivel inserir a data.\n");
                break;

            case 2:
                printf("Digite a data que deseja remover: ");
                if (fgets(data, sizeof(data), stdin) == NULL)
                    break;

                data[strcspn(data, "\n")] = '\0';

                if (removerVetor(&vetor, data) == 1)
                    printf("Data removida com sucesso!\n");
                else
                    printf("Data nao encontrada!\n");
                break;

            case 3:
            {
                clock_t inicio, fim;
                double tempo;

                inicio = clock();

                ordenarVetor(&vetor);

                fim = clock();

                tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

                listarOrdenadoVetor(&vetor);

                printf("Tempo de ordenacao: %.6f segundos\n", tempo);

                break;
            }

            case 4:
                printf("Quantidade de datas: %d\n",
                       vetor.quantidade);
                break;

            case 5:
                iniciarVetor(&vetor);

                gerarDatas(&vetor, 1000);

                printf("1000 datas geradas com sucesso!!\n");

                break;

            case 6:
                testarDesempenho(&vetor);
                break;

            case 7:
                testarInsercao(&vetor);
                break;

            case 8:
                testarRemocao(&vetor);
                break;
            case 9:
                salvarDatas();
                break;

            case 10:
            {
                static char datas[100000][20];

                int quantidade = carregarDatas(datas);

                printf("Datas carregadas: %d\n", quantidade);

                break;
            }

            case 11:
                calcularCustoPonderado();
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
