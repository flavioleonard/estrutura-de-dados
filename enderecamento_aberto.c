#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

typedef struct RegistroTabela
{
    TRegistro registro;
    struct RegistroTabela *proximo;
    bool ocupado;
} TRegistroTabela;

int calcularHash(long long chaveCPF)
{   
    int k;
    int nove_digitos = chaveCPF / 100;
    
    srand((unsigned int)nove_digitos);
    int hash = rand();
    hash = (hash + k) % TAMANHO_TABELA;

    return hash;
}


TRegistroTabela *inicializa_tabela()
{
    TRegistroTabela *tabela = malloc(sizeof(TRegistroTabela) * TAMANHO_TABELA);
    if (tabela == NULL)
        return NULL;

    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        tabela[i].ocupado = false;
        tabela[i].proximo = NULL;
    }

    return tabela;
}

void inserir_na_tabela(TRegistroTabela *tabela, TRegistro registro)
{
    int hash = calcularHash(registro.cpf);
    int posicao = hash;

    while (tabela[posicao].ocupado)
    {
        posicao = (posicao + 1) % TAMANHO_TABELA;
        if (posicao == hash)
        {
            printf("Tabela cheia! Não é possível inserir o registro de CPF %lld\n", registro.cpf);
            return;
        }
    }

    tabela[posicao].registro = registro;
    tabela[posicao].ocupado = true;
    tabela[posicao].proximo = NULL;

    if (posicao != hash)
    {
        int i = hash;
        while (true)
        {
            if (tabela[i].ocupado && (tabela[i].registro.cpf / 100 == registro.cpf / 100))
            {
                tabela[i].proximo = &tabela[posicao];
                break;
            }
            i = (i + 1) % TAMANHO_TABELA;
            if (i == posicao)
                break;
        }
    }
}

void carregar_e_inserir(TRegistroTabela *tabela)
{
    FILE *arquivo = fopen("registros.dat", "rb");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    TRegistro reg;
    int contador = 0;

    while (fread(&reg, sizeof(TRegistro), 1, arquivo) == 1)
    {
        inserir_na_tabela(tabela, reg);
        contador++;
        if (contador % 1000 == 0)
            printf("%d registros inseridos...\n", contador);
    }

    fclose(arquivo);
    printf("Inserção concluída: %d registros.\n", contador);
}

void exibir_tabela(TRegistroTabela *tabela)
{
    printf("\n--- TABELA HASH ---\n");
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        if (tabela[i].ocupado)
        {
            printf("Proximo %d | CPF: %lld | Nota: %d | Nome: %.*s\n", i,
                   tabela[i].registro.cpf,
                   tabela[i].registro.nota,
                   TAM_NOME, tabela[i].registro.nome);
            if (tabela[i].proximo != NULL)
            {
                printf("    -> Encadeado com CPF: %lld\n", tabela[i].proximo->registro.cpf);
            }
        }
    }
}


int main()
{
    TRegistroTabela *tabela = inicializa_tabela();
    if (tabela == NULL)
    {
        perror("Erro ao alocar memória para a tabela");
        return 1;
    }

    //carregar_e_inserir(tabela); 

    ///exibir_tabela(tabela); 

    int hash = calcularHash(123456789);
    printf("%d\n", hash);
    int hash2 = calcularHash(987654321);
    printf("%d\n", hash2);




    free(tabela);
    return 0;
}
