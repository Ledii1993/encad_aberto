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
        printf("Erro ao abrir o arquivo da tabela Meta");
        exit(1);
    }

    if ((dados = fopen("dados.dat", "r+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela Dados");
        exit(1);
    }

    printf("Digite o método deseja usar:\n 1 - Incremento Linear \n 2 - Incremento Quadrático \n 3 - Hash Duplo\n");
    scanf("%d", &metodo);

    if (metodo < 1 || metodo > 3) {
        printf("ERRO: Digite uma opção válida!");
    } else {
        prompt(meta, dados, metodo);
    }

    return 0;
}
