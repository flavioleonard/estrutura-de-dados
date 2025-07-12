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

// Função hash com sondagem linear
int hash(long long cpf, int m, int k) {
    srand(cpf);
    return (rand() % m + k) % m;
}

// Insere um novo registro na tabela hash
void insere_registro() {
    TRegistro novo;
    printf("Digite o CPF: ");
    scanf("%lld", &novo.cpf);
    printf("Digite o nome: ");
    scanf(" %[^\n]", novo.nome);
    printf("Digite a nota: ");
    scanf("%d", &novo.nota);

    FILE *arquivo = fopen("tabela_hash.dat", "rb+");
    if (!arquivo) {
        perror("Erro ao abrir arquivo tabela_hash.dat");
        return;
    }

    TRegistro reg;
    int k = 0, pos;

    while (k < TAMANHO_TABELA) {
        pos = hash(novo.cpf, TAMANHO_TABELA, k);
        fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
        fread(&reg, sizeof(TRegistro), 1, arquivo);

        if (reg.cpf == novo.cpf) {
            printf("CPF %lld ja existe na tabela!\n", novo.cpf);
            fclose(arquivo);
            return;
        }
        else if (reg.cpf == 0) {
            fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
            fwrite(&novo, sizeof(TRegistro), 1, arquivo);
            printf("Registro inserido na posicao %d.\n", pos);
            fclose(arquivo);
            return;
        }
        k++;
    }

    printf("Tabela cheia! Nao foi possivel inserir.\n");
    fclose(arquivo);
}

// Busca um registro na tabela hash
void busca_registro() {
    long long cpf;
    printf("Digite o CPF para buscar: ");
    scanf("%lld", &cpf);

    FILE *arquivo = fopen("tabela_hash.dat", "rb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo tabela_hash.dat");
        return;
    }

    TRegistro reg;
    int k = 0, pos, colisoes = 0;

    while (k < TAMANHO_TABELA) {
        pos = hash(cpf, TAMANHO_TABELA, k);
        fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
        fread(&reg, sizeof(TRegistro), 1, arquivo);

        if (reg.cpf == cpf) {
            printf("\nRegistro encontrado:\n");
            printf("Posicao: %d | Colisoes: %d\n", pos, colisoes);
            printf("CPF: %lld\nNome: %s\nNota: %d\n", reg.cpf, reg.nome, reg.nota);
            fclose(arquivo);
            return;
        }
        else if (reg.cpf == 0) {
            break;
        }
        colisoes++;
        k++;
    }

    printf("CPF %lld nao encontrado (colisoes: %d).\n", cpf, colisoes);
    fclose(arquivo);
}

// Remove um registro da tabela hash
void remove_registro() {
    long long cpf;
    printf("Digite o CPF para remover: ");
    scanf("%lld", &cpf);

    FILE *arquivo = fopen("tabela_hash.dat", "rb+");
    if (!arquivo) {
        perror("Erro ao abrir arquivo tabela_hash.dat");
        return;
    }

    TRegistro reg;
    int k = 0, pos;

    while (k < TAMANHO_TABELA) {
        pos = hash(cpf, TAMANHO_TABELA, k);
        fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
        fread(&reg, sizeof(TRegistro), 1, arquivo);

        if (reg.cpf == cpf) {
            TRegistro vazio = {0};
            fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
            fwrite(&vazio, sizeof(TRegistro), 1, arquivo);
            printf("Registro removido da posicao %d.\n", pos);
            fclose(arquivo);
            return;
        }
        else if (reg.cpf == 0) {
            break;
        }
        k++;
    }

    printf("CPF %lld nao encontrado para remocao.\n", cpf);
    fclose(arquivo);
}

// Imprime todos os registros não vazios
void imprime_tabela() {
    FILE *arquivo = fopen("tabela_hash.dat", "rb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo tabela_hash.dat");
        return;
    }

    TRegistro reg;
    int count = 0;
    printf("\n=== CONTEUDO DA TABELA HASH ===\n");
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        fseek(arquivo, i * sizeof(TRegistro), SEEK_SET);
        fread(&reg, sizeof(TRegistro), 1, arquivo);

        if (reg.cpf != 0) {
            printf("[%d] CPF: %lld | Nome: %s | Nota: %d\n", 
                  i, reg.cpf, reg.nome, reg.nota);
            count++;
        }
    }
    printf("=== TOTAL: %d registros ===\n", count);
    fclose(arquivo);
}

// Menu interativo
void menu() {
    int opcao;
    do {
        printf("\n=== MENU ===\n");
        printf("1. Inserir registro\n");
        printf("2. Buscar registro\n");
        printf("3. Remover registro\n");
        printf("4. Imprimir tabela\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: insere_registro(); break;
            case 2: busca_registro(); break;
            case 3: remove_registro(); break;
            case 4: imprime_tabela(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

int main() {
    menu();
    return 0;
}