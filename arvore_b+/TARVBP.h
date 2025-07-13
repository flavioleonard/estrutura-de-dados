#ifndef TARVBP_H
#define TARVBP_H

#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

#define TAM 2

typedef struct
{
    int folha;
    int num_chaves;
    char chaves[TAM * 2 - 1][13]; // Chave: string dos 9 primeiros dígitos do CPF
    long long filhos[TAM * 2];
    long long reg[TAM * 2 - 1];
    long long end;
    long long proximo_no;
} TARVBP;

TARVBP TARVBP_cria(char *idx);
void ler_no(char *arq, long long reg, TARVBP *no);
long long busca_cpf(char *arq_idx, char *cpf, long long reg);
void escrever_no(char *arq, long long reg, TARVBP *no);

long long TARVBP_insere(long reg, Dados *d, int t, char *idx, char *dados);
long long TARVBP_remove(long reg, char *chave, int t, char *idx, char *dados, int *diminuiu);

#endif