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
    Registro *novo;

    while (1) {
            printf("--------------------------------------------------\n");
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

            novo = criarRegistro(codigo, nome);

            double tempoGasto1 = 0.0;

            struct timespec inicio1, fim1;
            clock_gettime(CLOCK_MONOTONIC, &inicio1);
            inserirRegistro(dados, novo, metodo);
            clock_gettime(CLOCK_MONOTONIC, &fim1);
            tempoGasto1 = (fim1.tv_sec - inicio1.tv_sec) + (fim1.tv_nsec - inicio1.tv_nsec) / 1e9;

            printf("TEMPO GASTO: %f ms\n", tempoGasto1 * 1000);

            free(novo);
            //printf("fechando arquivos e ponteiros");
            break;
        case (2):
            printf("Digite o codigo a deletar:\n");
            scanf("%d", &codigo);

            double tempoGasto2 = 0.0;

            struct timespec inicio2, fim2;
            clock_gettime(CLOCK_MONOTONIC, &inicio2);
            deletarRegistro(dados, codigo, metodo);
            clock_gettime(CLOCK_MONOTONIC, &fim2);
            tempoGasto2 = (fim2.tv_sec - inicio2.tv_sec) + (fim2.tv_nsec - inicio2.tv_nsec) / 1e9;
            
            printf("TEMPO GASTO: %f ms\n", tempoGasto2 * 1000);

            free(novo);
            break;
        case (3):
            printf("Digite o codigo desejado:\n");
            scanf("%d", &codigo);

            double tempoGasto3 = 0.0;

            struct timespec inicio3, fim3;
            clock_gettime(CLOCK_MONOTONIC, &inicio3);
            novo = encontrarRegistro(dados, codigo, metodo);
            clock_gettime(CLOCK_MONOTONIC, &fim3);
            tempoGasto3 = (fim3.tv_sec - inicio3.tv_sec) + (fim3.tv_nsec - inicio3.tv_nsec) / 1e9;


            if(novo->codigo == -1){
                printf("ERRO: REGISTRO NAO ENCONTRADO!");
            }
            else{
                printf("REGISTRO: %s \n", novo->nome);
                printf("CÓDIGO: %d \n", novo->codigo);
                printf("TEMPO GASTO: %f ms\n", tempoGasto3 * 1000);
            }
            break;
        case (4):
            zerarRegistros(meta, dados);
            free(novo);
            break;
        case (5):
            mostrarRegistros(dados, meta);
            break;
        case (6):
        free(novo);
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
