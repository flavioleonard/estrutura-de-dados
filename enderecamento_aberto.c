#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_REGISTROS 10000
#define TAMANHO_TABELA 100000
#define TAM_NOME 50

typedef struct Registro {
    char nome[TAM_NOME];
    long long int cpf;
    int nota;
} TRegistro;

typedef TRegistro* Hash;

int hash(long long cpf, int m, int k) {
    unsigned int chave = (unsigned int)(cpf / 100); // nove primeiros dígitos
    srand(chave); // reinicia com base no CPF
    int valor_hash = rand(); // gera sempre o mesmo valor para o mesmo CPF
    return (valor_hash + k) % m;
}


int busca(Hash *tab, int m, long long cpf, int *achou) {
    *achou = 0;
    int end = -1;
    int pos_livre = -1;
    int k = 0;

    while (k < m) {
        end = hash(cpf, m, k);
        if (tab[end] != NULL && tab[end]->cpf == cpf) {
            *achou = 1;
            break;
        } else if (tab[end] == NULL && pos_livre == -1) {
            pos_livre = end;
        }
        k++;
    }

    return *achou ? end : pos_livre;
}

TRegistro* busca_registro(Hash *tab, int m, long long cpf) {
    int achou;
    int pos = busca(tab, m, cpf, &achou);

    if (achou) {
        return tab[pos];  
    } else {
        return NULL;      
    }
}


TRegistro *aloca_registro(char *nome, long long cpf, int nota) {
    TRegistro *novo = malloc(sizeof(TRegistro));
    if (novo) {
        strcpy(novo->nome, nome);
        novo->cpf = cpf;
        novo->nota = nota;
    }
    return novo;
}

void insere(Hash *tab, int m, char *nome, long long cpf, int nota) {
    int achou;
    int end = busca(tab, m, cpf, &achou);

    if (!achou) {
        if (end != -1) {
            tab[end] = aloca_registro(nome, cpf, nota);
        } else {
            printf("Overflow. Não foi possível inserir CPF %lld\n", cpf);
        }
    } else {
        printf("CPF já existe. Inserção inválida!\n");
    }
}

void exclui(Hash *tab, int m, long long cpf) {
    int achou;
    int end = busca(tab, m, cpf, &achou);
    if (achou) {
        free(tab[end]);
        tab[end] = NULL;
    } else {
        printf("CPF %lld não encontrado. Remoção não realizada!\n", cpf);
    }
}

void inicializa(Hash *tab, int m) {
    for (int i = 0; i < m; i++) {
        tab[i] = NULL;
    }
}

void carrega_registros_na_tabela(Hash *tabela) {
    FILE *arquivo = fopen("registros.dat", "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    TRegistro reg;
    while (fread(&reg, sizeof(TRegistro), 1, arquivo) == 1) {
        insere(tabela, TAMANHO_TABELA, reg.nome, reg.cpf, reg.nota);
    }

    fclose(arquivo);
}


void imprimir_tabela(Hash *tabela) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        if (tabela[i] != NULL) {
            printf("Posicao %d | CPF: %lld | Nome: %s | Nota: %d\n",
                   i,
                   tabela[i]->cpf,
                   tabela[i]->nome,
                   tabela[i]->nota);
        }
    }
}

void liberar_tabela(Hash *tabela) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        if (tabela[i] != NULL) {
            free(tabela[i]);
        }
    }
}

int main() {
    Hash *tabela = malloc(sizeof(Hash) * TAMANHO_TABELA);
    if (!tabela) {
        perror("Erro de alocação");
        return 1;
    }

    inicializa(tabela, TAMANHO_TABELA);

    carrega_registros_na_tabela(tabela);

    long long cpf_alvo = 5841347206LL;
    int achou;
    int pos = busca(tabela, TAMANHO_TABELA, cpf_alvo, &achou);

    if (achou) {
        printf("\nRegistro encontrado na posição %d:\n", pos);
        printf("CPF: %lld | Nome: %s | Nota: %d\n",
               tabela[pos]->cpf,
               tabela[pos]->nome,
               tabela[pos]->nota);
    } else {
        printf("\nCPF %lld não encontrado na tabela.\n", cpf_alvo);
    }

    int busc = busca(tabela, TAMANHO_TABELA, cpf_alvo, &achou);

    if (achou) {
        printf("\nRegistro encontrado na posição %d:\n", pos);
        printf("CPF: %lld | Nome: %s | Nota: %d\n",
               tabela[pos]->cpf,
               tabela[pos]->nome,
               tabela[pos]->nota);
    } else {
        printf("\nCPF %lld não encontrado na tabela.\n", cpf_alvo);
    }

    liberar_tabela(tabela);
    free(tabela);
    return 0;
}


