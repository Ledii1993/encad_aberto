#include <stdio.h>
#include <stdlib.h>
#include "registros.h"
#include "prompt.h"

int main() {
    FILE *meta;
    FILE *dados;
    int metodo;

    //system("cls");

    if ((meta = fopen("meta.dat", "r+b")) == NULL) {
        printf("ERRO: NAO FOI POSSIVEL ABRIR O ARQUIVO META.");
        exit(1);
    }

    if ((dados = fopen("dados.dat", "r+b")) == NULL) {
        printf("ERRO: NAO FOI POSSIVEL ABRIR O ARQUIVO TABELA DE DADOS.");
        exit(1);
    }

    printf("\nDigite o metodo deseja usar:\n [1] INCREMENTO LINEAR \n [2] INCREMENTO QUADRATICO \n [3] HASH DUPLO\n");
    printf("Metodo: ");
    scanf("%d", &metodo);
    printf("\n");

    if (metodo < 1 || metodo > 3) {
        printf("ERRO: DIGITE UMA ENTRADA VALIDA!!");
    } else {
        prompt(meta, dados, metodo);
    }

    return 0;
}
