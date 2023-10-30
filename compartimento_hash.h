#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"

#define TAMANHO_HASH 8
#define REGISTRO_DADOS "registros.dat"
#define METADADOS "meta.dat"

Registro *criarRegistro(int codigoRegistro, char *nomeRegistro) {
    Registro *novo = (Registro *)malloc(sizeof(Registro));
    novo->codigo = codigoRegistro;
    strcpy(novo->nome, nomeRegistro);
    novo->status = 1;
    return novo;
}

int incrementoLinear(int tentativa, int codigo) {
    int x = codigo % TAMANHO_HASH;
    return (x + tentativa) % TAMANHO_HASH;
}

int incrementoQuadratico(int tentativa, int codigo) {
    int x, y;
    if (tentativa == 0) {
        return codigo % TAMANHO_HASH;
    } else {
        x = incrementoQuadratico(tentativa - 1, codigo);
        y = (x + tentativa) % TAMANHO_HASH;
        return y;
    }
}

int hashDuplo(int tentativa, int codigo) {
    int x = codigo % TAMANHO_HASH;
    int y = 1 + (codigo % (TAMANHO_HASH - 1));
    return (x + tentativa * y) % TAMANHO_HASH;
}

Registro *encontrarRegistro(FILE *dados, int codigo, int metodo) {
    Registro *temp = (Registro *)malloc(sizeof(Registro));
    int val = 0, posicao, tentativa = 0;

    while (val == 0) {
        rewind(dados);
        if (metodo == 1) {
            posicao = incrementoLinear(tentativa, codigo);
        } else if (metodo == 2) {
            posicao = incrementoQuadratico(tentativa, codigo);
        } else {
            posicao = hashDuplo(tentativa, codigo);
        }

        fseek(dados, sizeof(Registro) * posicao, SEEK_SET);

        fread(&temp->codigo, sizeof(int), 1, dados);
        fread(temp->nome, sizeof(char), sizeof(temp->nome), dados);
        fread(&temp->status, sizeof(int), 1, dados);

        if (temp->codigo == codigo) {
            return temp; // Retorna o registro encontrado
        }

        if (tentativa >= TAMANHO_HASH) {
            temp->codigo = -1;
            return temp;
        }

        tentativa++;
    }
}

void inserirRegistro(FILE *dados, Registro *info, int metodo) {
    Registro *temp;
    int posicao, tentativa, totalColisoes, val = 0;

    tentativa = 0;

    temp = encontrarRegistro(dados, info->codigo, metodo);

    if (temp->codigo == info->codigo) {
        printf("CODIGO: %d [EM USO].\n", temp->codigo);
        return;
    }

    while (val == 0) {
        if (metodo == 1) {
            posicao = incrementoLinear(tentativa, info->codigo);
        } else if (metodo == 2) {
            posicao = incrementoQuadratico(tentativa, info->codigo);
        } else {
            posicao = hashDuplo(tentativa, info->codigo);
        }

        printf("POSICAO: %d \n", posicao);
        printf("LOOPs: %d \n", tentativa);

        rewind(dados);
        fseek(dados, sizeof(Registro) * posicao, SEEK_SET);

        fread(&temp->codigo, sizeof(int), 1, dados);
        fread(temp->nome, sizeof(char), sizeof(temp->nome), dados);
        fread(&temp->status, sizeof(int), 1, dados);

        if (temp->status <= 0) {
            val = 1;
        } else if (tentativa >= TAMANHO_HASH) {
            val = 3;
        }

        totalColisoes += tentativa;
        printf("COLISOES: %d \n", tentativa );
        tentativa++;
    }

    if (val != 3) {
        rewind(dados);
        fseek(dados, sizeof(Registro) * posicao, SEEK_SET);
        fwrite(&info->codigo, sizeof(int), 1, dados);
        fwrite(info->nome, sizeof(char), sizeof(info->nome), dados);
        fwrite(&info->status, sizeof(int), 1, dados);
    } else {
        printf("ERRO: SEM ESPACO DISPONIVEL\n");
    }

    printf("COLISOES: %d\n", totalColisoes);
}

void deletarRegistro(FILE *dados, int codigo, int metodo) {
    int val = 0, posicao, tentativa = 0;
    Registro *atual = (Registro *)malloc(sizeof(Registro));

    while (val == 0) {
        if (metodo == 1) {
            posicao = incrementoLinear(tentativa, codigo);
        } else if (metodo == 2) {
            posicao = incrementoQuadratico(tentativa, codigo);
        } else {
            posicao = hashDuplo(tentativa, codigo);
        }

        rewind(dados);
        fseek(dados, sizeof(Registro) * posicao, SEEK_SET);
        fread(&atual->codigo, sizeof(int), 1, dados);
        fread(atual->nome, sizeof(char), sizeof(atual->nome), dados);
        fread(&atual->status, sizeof(int), 1, dados);

        if (atual->codigo == codigo) {
            val = 1;
            atual->codigo = -2;
            strcpy(atual->nome, "----");
            atual->status = 0;

            rewind(dados);
            fseek(dados, sizeof(Registro) * posicao, SEEK_SET);

            fwrite(&atual->codigo, sizeof(int), 1, dados);
            fwrite(atual->nome, sizeof(char), sizeof(atual->nome), dados);
            fwrite(&atual->status, sizeof(int), 1, dados);

            rewind(dados);
            fseek(dados, sizeof(Registro) * posicao, SEEK_SET);

            fread(&atual->codigo, sizeof(int), 1, dados);
            fread(atual->nome, sizeof(char), sizeof(atual->nome), dados);

            printf("SUCESSO: REGISTRO DELETADO.\n");
            return;
        } else if (tentativa >= TAMANHO_HASH) {
            val = -1;
            printf("ERRO: REGISTRO NAO ENCONTRADO.\n");
            free(atual);
            break;
        } else {
            tentativa++;
        }
    }

    free(atual);
}

void mostrarRegistros(FILE *dados, FILE *meta) {
    Registro *registro = (Registro *)malloc(sizeof(Registro));
    int contador, i = 0;

    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);

    if (contador == 0) {
        printf("FLAG: SEM REGISTROS.\n");
        free(registro);
        return;
    }

    printf("\nREGISTROS\n");
    rewind(dados);

    for (i = 0; i < TAMANHO_HASH; i++) {
        rewind(dados);
        fseek(dados, sizeof(Registro) * i, SEEK_SET);
        fread(&registro->codigo, sizeof(int), 1, dados);

        if (registro->codigo < 0) {
            continue;
        }

        fread(registro->nome, sizeof(char), sizeof(registro->nome), dados);
        fread(&registro->status, sizeof(int), 1, dados);

        printf("--------------------------------------------------\n");
        printf("POSIÇÃO: %d\n", i);
        printf("CÓDIGO: %d\n", registro->codigo);
        printf("REGISTRO: %s\n", registro->nome);
    }

    free(registro);
}

void zerarRegistros(FILE *meta, FILE *dados) {
    int contador = TAMANHO_HASH;
    int novo;
    //int a = -1;
    Registro *atual = (Registro *)malloc(sizeof(Registro));
    //int b, status, prox;

    atual->codigo = -1;
    strcpy(atual->nome, "-----");
    atual->status = 0;

    fclose(meta);

    if ((meta = fopen(METADADOS, "wb")) == NULL) {
        printf("ERRO: NAO FOI POSSIVEL ABRIR O ARQUIVO META.\n");
        exit(1);
    }

    for (int i = 0; i < TAMANHO_HASH; i++) {
        rewind(dados);
        fseek(dados, sizeof(Registro) * i, SEEK_SET);
        fwrite(&atual->codigo, sizeof(int), 1, dados);
        fwrite(atual->nome, sizeof(Registro), sizeof(atual->nome), dados);
        fwrite(&atual->status, sizeof(int), 1, dados);
    }

    rewind(meta);
    fwrite(&contador, sizeof(int), 1, meta);

    fclose(meta);

    if ((meta = fopen(METADADOS, "r+b")) == NULL) {
        printf("ERRO: NAO FOI POSSIVEL ABRIR O ARQUIVO META.\n");
        exit(1);
    }

    rewind(meta);
    fread(&novo, sizeof(int), 1, meta);

    printf("TABELA DE REGISTROS ZERADA!! \n");
    printf("ARQUIVOS ZERADOS!! \n\n");

    rewind(dados);
    free(atual);
}
