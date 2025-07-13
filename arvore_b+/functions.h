#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define CPF 12

typedef struct
{
    char cpf[13];
    char nome[50];
    int nota_final;
} Dados;

extern const char *nomes[100];
extern const char *sobrenomes[100];
extern const char *a[100];
extern const char *b[100];

void gerador(const char **n, const char **s, const char *arquivo);
void gerador_nota(const char *arquivo);
void leitura_escrita(const char *arquivo_bin, const char *arquivo_txt);
void leitura_escrita_nota(const char *arquivo_bin, const char *arquivo_txt);

// Inicializa arquivos de índice e dados (implementado em functions.c)
void inic_arq();

#endif // FUNCTIONS_H