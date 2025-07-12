#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_TABELA 100000
#define TAM_NOME 50

typedef struct Registro {
    char nome[TAM_NOME];
    long long int cpf;
    int nota;
} TRegistro;

// Variável global para contar o total de colisões
long long total_colisoes = 0;

// Função hash com sondagem linear
int hash(long long cpf, int m, int k) {
    srand(cpf);
    return (rand() % m + k) % m;
}

// Inicializa o arquivo da tabela hash com registros vazios
void inicializa_tabela_hash() {
    FILE *arquivo = fopen("tabela_hash.dat", "wb");
    if (!arquivo) {
        perror("Erro ao criar arquivo tabela_hash.dat");
        exit(1);
    }

    TRegistro vazio;
    memset(&vazio, 0, sizeof(TRegistro));

    for (int i = 0; i < TAMANHO_TABELA; i++) {
        fwrite(&vazio, sizeof(TRegistro), 1, arquivo);
    }

    fclose(arquivo);
}

// Insere um registro no arquivo da tabela hash e conta colisões
void insere_tabela_hash(char *nome, long long cpf, int nota) {
    FILE *arquivo = fopen("tabela_hash.dat", "rb+");
    if (!arquivo) {
        perror("Erro ao abrir arquivo tabela_hash.dat para insercao");
        return;
    }

    TRegistro reg;
    int k = 0, pos;

    while (k < TAMANHO_TABELA) {
        pos = hash(cpf, TAMANHO_TABELA, k);
        fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
        fread(&reg, sizeof(TRegistro), 1, arquivo);

        if (reg.cpf == cpf) {
            fclose(arquivo);
            return;
        }
        else if (reg.cpf == 0) {
            TRegistro novo;
            strncpy(novo.nome, nome, TAM_NOME - 1);
            novo.nome[TAM_NOME - 1] = '\0';
            novo.cpf = cpf;
            novo.nota = nota;

            fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
            fwrite(&novo, sizeof(TRegistro), 1, arquivo);
            fclose(arquivo);
            
            // Adiciona o número de colisões deste registro ao total
            total_colisoes += k;
            return;
        }
        k++;
    }

    printf("Overflow. Nao foi possivel inserir CPF %lld.\n", cpf);
    fclose(arquivo);
}

// Lê registros de um arquivo de origem e insere na tabela hash
void le_registros_origem_e_insere_na_hash(const char *nome_arquivo_origem) {
    FILE *arquivo_origem = fopen(nome_arquivo_origem, "rb");
    if (!arquivo_origem) {
        perror("Erro ao abrir o arquivo de registros de origem");
        exit(1);
    }

    TRegistro reg_origem;
    int count = 0;
    while (fread(&reg_origem, sizeof(TRegistro), 1, arquivo_origem) == 1) {
        insere_tabela_hash(reg_origem.nome, reg_origem.cpf, reg_origem.nota);
        count++;
    }

    printf("Total de %d registros inseridos na tabela hash.\n", count);
    printf("Total de colisoes durante a insercao: %lld\n", total_colisoes);
    fclose(arquivo_origem);
}

int main() {
    printf("Criando tabela hash...\n");
    inicializa_tabela_hash();
    
    printf("Carregando registros...\n");
    le_registros_origem_e_insere_na_hash("../registros.dat");
    
    printf("Tabela hash criada com sucesso no arquivo 'tabela_hash.dat'.\n");
    return 0;
}