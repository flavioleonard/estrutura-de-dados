#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TOTAL_REGISTROS 10000
#define TAMANHO_TABELA 100000
#define TAM_NOME 50

typedef struct Registro
{
    char nome[TAM_NOME];
    long long int cpf;
    int nota;
} TRegistro;

typedef struct RegistroTabela
{
    TRegistro registro;
    struct registroTabela *proximo;
    bool ocupado;
} TRegistroTabela;

int calcularHash(long long chaveCPF)
{
    long long nove_digitos = chaveCPF / 100;
    
    srand((unsigned int)nove_digitos); 
    
    int hashValue = rand();
    
    return hashValue % TAMANHO_TABELA; 
}

void abre_registros()
{
    FILE *arquivo = fopen("registros.dat", "rb");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir arquivo para leitura");
        exit(1);
    }
    TRegistro reg;
    int contador = 0;
    while (fread(&reg, sizeof(TRegistro), 1, arquivo) == 1)
    {
        int hash = calcularHash(reg.cpf);
        printf("CPF: %lld (Hash: %d)\n", reg.cpf, hash);
    }
    fclose(arquivo);
}

TRegistroTabela *inicializa_tabela()
{
    TRegistroTabela *tabela = malloc(sizeof(TRegistroTabela) * TAMANHO_TABELA);
    return tabela;
}

int main()
{   
    TRegistroTabela *tabela = inicializa_tabela();
    if (tabela == NULL)
    {
        perror("Erro ao alocar memoria para tabela de registros");
        exit(1);
    }
    abre_registros();

    return 0;
}