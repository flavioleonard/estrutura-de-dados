#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_REGISTROS 10000
#define TAMANHO_TABELA 100000
#define TAM_NOME 50

typedef struct Registro
{
    char nome[TAM_NOME];
    long long int cpf;
    int nota;
} TRegistro;

int hash(long long cpf, int m, int k) {
    return (cpf % m + k) % m;  // Sondagem linear simples
}

void inicializa_arquivo()
{
    FILE *arquivo = fopen("registros.dat", "wb");
    if (!arquivo)
    {
        perror("Erro ao criar arquivo");
        exit(1);
    }

    TRegistro vazio;
    memset(&vazio, 0, sizeof(TRegistro));

    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        fwrite(&vazio, sizeof(TRegistro), 1, arquivo);
    }

    fclose(arquivo);
}

void insere_arquivo(char *nome, long long cpf, int nota)
{
    FILE *arquivo = fopen("registros.dat", "rb+");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo");
        return;
    }

    TRegistro reg;
    int k = 0, pos;

    while (k < TAMANHO_TABELA)
    {
        pos = hash(cpf, TAMANHO_TABELA, k);
        fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
        fread(&reg, sizeof(TRegistro), 1, arquivo);

        if (reg.cpf == cpf)
        {
            printf("CPF ja existe. Insercao invalida!\n");
            fclose(arquivo);
            return;
        }
        else if (reg.cpf == 0)
        {
            // Posicao vazia encontrada, podemos inserir
            TRegistro novo;
            strncpy(novo.nome, nome, TAM_NOME);
            novo.cpf = cpf;
            novo.nota = nota;

            fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
            fwrite(&novo, sizeof(TRegistro), 1, arquivo);
            fclose(arquivo);
            return;
        }

        k++;
    }

    // Se chegou aqui, percorreu toda a tabela e nao encontrou espaco
    printf("Overflow. Nao foi possivel inserir CPF %lld\n", cpf);
    fclose(arquivo);
}

void busca_arquivo(long long cpf)
{
    FILE *arquivo = fopen("registros.dat", "rb");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo");
        return;
    }

    TRegistro reg;
    int k = 0, pos;

    while (k < TAMANHO_TABELA)
    {
        pos = hash(cpf, TAMANHO_TABELA, k);
        fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
        fread(&reg, sizeof(TRegistro), 1, arquivo);

        if (reg.cpf == cpf)
        {
            printf("\nRegistro encontrado na posicao %d:\n", pos);
            printf("CPF: %lld | Nome: %s | Nota: %d\n", reg.cpf, reg.nome, reg.nota);
            fclose(arquivo);
            return;
        }
        else if (reg.cpf == 0)
        {
            break;
        }

        k++;
    }

    printf("CPF %lld nao encontrado.\n", cpf);
    fclose(arquivo);
}

void exclui_arquivo(long long cpf)
{
    FILE *arquivo = fopen("registros.dat", "rb+");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo");
        return;
    }

    TRegistro reg;
    int k = 0, pos;

    while (k < TAMANHO_TABELA)
    {
        pos = hash(cpf, TAMANHO_TABELA, k);
        fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
        fread(&reg, sizeof(TRegistro), 1, arquivo);

        if (reg.cpf == cpf)
        {
            TRegistro vazio;
            memset(&vazio, 0, sizeof(TRegistro));

            fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
            fwrite(&vazio, sizeof(TRegistro), 1, arquivo);

            printf("Registro com CPF %lld removido da posicao %d.\n", cpf, pos);
            fclose(arquivo);
            return;
        }
        else if (reg.cpf == 0)
        {
            break;
        }

        k++;
    }

    printf("CPF %lld nao encontrado para remocao.\n", cpf);
    fclose(arquivo);
}

void imprimir_arquivo()
{
    FILE *arquivo = fopen("registros.dat", "rb");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo");
        return;
    }

    TRegistro reg;
    int count = 0;
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        fseek(arquivo, i * sizeof(TRegistro), SEEK_SET);
        fread(&reg, sizeof(TRegistro), 1, arquivo);

        if (reg.cpf != 0)
        {
            printf("Posicao %d | CPF: %lld | Nome: %s | Nota: %d\n", i, reg.cpf, reg.nome, reg.nota);
            count++;
        }
    }

    printf("Total de registros: %d\n", count);
    fclose(arquivo);
}

int main()
{
    inicializa_arquivo();
    
    insere_arquivo("Joao da Silva", 12345678901, 85);
    insere_arquivo("Maria Oliveira", 5841347206, 90);
    insere_arquivo("Carlos Pereira", 9988776655, 75);

    busca_arquivo(5841347206LL);

    exclui_arquivo(12345678901LL);

    imprimir_arquivo();

    return 0;
}