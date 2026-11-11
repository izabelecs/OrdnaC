#include "testes.h"
#include <stdio.h>
#include "arvore.h"

int main()
{
    Arvore arvore;
    int opcao;
    char data[20];

    iniciarArvore(&arvore);

    do
    {
        printf("\n===== ARVORE BINARIA DE BUSCA =====\n");
        printf("1 - Inserir data\n");
        printf("2 - Remover data\n");
        printf("3 - Listar datas ordenadas\n");
        printf("4 - Mostrar quantidade\n");
        printf("5 - Testar desempenho da arvore\n");
        printf("6 - Calcular custo ponderado\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1)
        {
            opcao = 0;
        }

        switch (opcao)
        {
            case 1:
                printf("Digite a data (AAAA-MM-DD HH:MM:SS): ");
                scanf(" %19[^\n]", data);

                if (inserirArvore(&arvore, data) == 1)
                    printf("Data inserida!\n");
                else
                    printf("Erro ao inserir!\n");

                break;

            case 2:
                printf("Digite a data para remover: ");
                scanf(" %19[^\n]", data);

                if (removerArvore(&arvore, data) == 1)
                    printf("Data removida!\n");
                else
                    printf("Data nao encontrada!\n");

                break;

            case 3:
                listarOrdenadoArvore(&arvore);
                break;

            case 4:
                printf("Quantidade: %d\n", arvore.quantidade);
                break;

            case 5:
                testarDesempenhoArvore();
                break;

            case 6:
                calcularCustoPonderadoArvore();
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    liberarArvore(&arvore);

    return 0;
}
