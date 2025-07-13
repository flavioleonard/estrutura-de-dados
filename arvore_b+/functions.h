
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct
{
    long long int cpf; // CPF como long long int (11 dígitos)
    char nome[50];
    int nota_final;
} Dados;

// Inicializa arquivos de índice e dados (implementado em functions.c)
void inic_arq();

#endif // FUNCTIONS_H