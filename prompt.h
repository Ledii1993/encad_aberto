#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "compartimento_hash.h"
#include <time.h>

// Ao iniciar o codigo, sera aberto um prompt com as seguintes informacoes
void prompt(FILE *meta, FILE *dados, int metodo) {
    int escolha, codigo;
    char nome[100];
    Registro *new;

    while (1) {
            printf("\n--------------------------------------------------\n");
            printf("\nPor favor, selecione uma opcao:\n\nPara inserir um novo Registro, digite '1'.\nPara deletar um Registro, digite '2'.\nPara encontrar um Registro, digite '3'.\nPara reiniciar a tabela, digite '4' (CUIDADO: TODOS OS ESPACOS DA TABELA SERAO ZERADOS)\n\nSe quiser ver os Registros, digite '5'.\n\nCaso deseje sair do programa, digite '6'\n");
            printf("\nDigite uma das opcoes acima: ");
            scanf("%d", &escolha);
            switch (escolha){
        case (1) :
            printf("Digite o nome do registro a adicionar: ");
            scanf("%s", nome);
            printf("Digite o codigo a se relacionar: ");
            scanf("%d", &codigo);
            printf("\n");

            new = criarRegistro(codigo, nome);

            double time_spent1 = 0.0;

            clock_t begin1 = clock();
            inserirRegistro(dados, new, metodo);
            clock_t end1 = clock();
            time_spent1 += (double)(end1 - begin1) / CLOCKS_PER_SEC;

            printf("TEMPO GASTO: %f ms\n", time_spent1 * 1000);

            free(new);
            //printf("fechando arquivos e ponteiros");
            break;
        case (2):
            printf("Digite o codigo a deletar:\n");
            scanf("%d", &codigo);

            double tempoGasto2 = 0.0;

            clock_t begin2 = clock();
            deletarRegistro(dados, codigo, metodo);
            clock_t end2 = clock();

            tempoGasto2 += (double)(end2 - begin2) / CLOCKS_PER_SEC;
            printf("TEMPO GASTO: %f ms\n", tempoGasto2 * 1000);

            free(new);
            break;
        case (3):
            printf("Digite o codigo desejado:\n");
            scanf("%d", &codigo);

            double tempoGasto3 = 0.0;

            clock_t begin3 = clock();
            new = encontrarRegistro(dados, codigo, metodo);
            clock_t end3 = clock();

            tempoGasto3 += (double)(end3 - begin3) / CLOCKS_PER_SEC; 


            if(new->codigo == -1){
                printf("ERRO: REGISTRO NAO ENCONTRADO!");
            }
            else{
                printf("REGISTRO: %s \n", new->nome);
                printf("CÓDIGO: %d \n", new->codigo);
                printf("TEMPO GASTO: %f ms\n", tempoGasto2 * 1000);
            }
            break;
        case (4):
            zerarRegistros(meta, dados);
            free(new);
            break;
        case (5):
            mostrarRegistros(dados, meta);
            break;
        case (6):
        free(new);
            fclose(meta);
            fclose(dados);
            exit(1);
            //system("cls");
            break;
        default:
            printf("Por favor, selecione uma opçao:\n");
            break;
        }
    }
    system("pause");
}