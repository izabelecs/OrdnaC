#include "testes.h"
#include <stdio.h>
#include "lista.h"

int main()
{
    Lista lista;
    int opcao;
    char data[20];

    iniciarLista(&lista);

    do
    {
        printf("\n===== LISTA ENCADEADA ORDENADA =====\n");
        printf("1 - Inserir data\n");
        printf("2 - Remover data\n");
        printf("3 - Listar datas\n");
        printf("4 - Mostrar quantidade\n");
        printf("5 - Testar desempenho da lista\n");
        printf("6 - Calcular custo ponderado\n");
        printf("7 - Testar somente listagem\n");
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

                if (inserirLista(&lista, data) == 1)
                    printf("Data inserida!\n");
                else
                    printf("Erro ao inserir!\n");
                break;

            case 2:
                printf("Digite a data para remover: ");
                scanf(" %19[^\n]", data);

                if (removerLista(&lista, data) == 1)
                    printf("Data removida!\n");
                else
                    printf("Data nao encontrada!\n");
                break;

            case 3:
                listarOrdenadoLista(&lista);
                break;

            case 4:
                printf("Quantidade: %d\n", lista.quantidade);
                break;

            case 5:
                testarDesempenhoLista();
                break;

            case 6:
                calcularCustoPonderadoLista();
                break;

            case 7:
                testarSomenteListagemLista();
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    liberarLista(&lista);

    return 0;
}
