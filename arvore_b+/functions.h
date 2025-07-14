
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct
{
    char nome[50];
    long long int cpf;
    int nota;
} Dados;

// Inicializa arquivos de índice e dados (implementado em functions.c)
void inic_arq();

#endif // FUNCTIONS_H