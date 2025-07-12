#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_REGISTROS 10000 // Número de registros no seu arquivo de origem
#define TAMANHO_TABELA 100000 // Tamanho do seu arquivo da tabela hash
#define TAM_NOME 50

typedef struct Registro
{
    char nome[TAM_NOME];
    long long int cpf;
    int nota;
} TRegistro;

// Função hash com sondagem linear
int hash(long long cpf, int m, int k) {
    return (cpf % m + k) % m;
}

// Inicializa o arquivo da tabela hash (tabela_hash.dat) com registros vazios
void inicializa_tabela_hash()
{
    FILE *arquivo = fopen("tabela_hash.dat", "wb");
    if (!arquivo)
    {
        perror("Erro ao criar arquivo tabela_hash.dat");
        exit(1);
    }

    TRegistro vazio;
    memset(&vazio, 0, sizeof(TRegistro)); // Zera todos os bytes, definindo cpf como 0

    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        fwrite(&vazio, sizeof(TRegistro), 1, arquivo);
    }

    fclose(arquivo);
}

// Insere um registro no arquivo da tabela hash (tabela_hash.dat)
void insere_tabela_hash(char *nome, long long cpf, int nota)
{
    FILE *arquivo = fopen("tabela_hash.dat", "rb+");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo tabela_hash.dat para insercao");
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
            printf("CPF %lld ja existe na tabela hash. Insercao invalida!\n", cpf);
            fclose(arquivo);
            return;
        }
        else if (reg.cpf == 0) // Posição vazia (cpf == 0 indica um slot vazio)
        {
            TRegistro novo;
            strncpy(novo.nome, nome, TAM_NOME - 1); // Usa TAM_NOME - 1 para garantir terminação nula
            novo.nome[TAM_NOME - 1] = '\0';        // Terminação nula explícita
            novo.cpf = cpf;
            novo.nota = nota;

            fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
            fwrite(&novo, sizeof(TRegistro), 1, arquivo);
            fclose(arquivo);
            return;
        }

        k++; // Próxima tentativa de sondagem linear
    }

    // Se chegou aqui, percorreu toda a tabela e não encontrou espaço
    printf("Overflow. Nao foi possivel inserir CPF %lld. Tabela hash cheia ou nao ha espaco.\n", cpf);
    fclose(arquivo);
}

// Lê registros de um arquivo de origem (registros.dat)
// e os insere na tabela hash (tabela_hash.dat)
void le_registros_origem_e_insere_na_hash(const char *nome_arquivo_origem)
{
    FILE *arquivo_origem = fopen(nome_arquivo_origem, "rb"); // Abre o arquivo cujo nome foi passado como argumento
    if (!arquivo_origem)
    {
        perror("Erro ao abrir o arquivo de registros de origem");
        exit(1);
    }

    TRegistro reg_origem;
    int count = 0;
    // Lê um registro por vez até o final do arquivo
    while (fread(&reg_origem, sizeof(TRegistro), 1, arquivo_origem) == 1)
    {
        // Chama insere_tabela_hash com os dados lidos do arquivo registros.dat
        insere_tabela_hash(reg_origem.nome, reg_origem.cpf, reg_origem.nota);
        count++;
    }

    printf("Total de %d registros lidos do arquivo de origem e tentados inserir na tabela hash.\n", count);
    fclose(arquivo_origem);
}


// Busca por um registro no arquivo da tabela hash (tabela_hash.dat)
void busca_tabela_hash(long long cpf)
{
    FILE *arquivo = fopen("tabela_hash.dat", "rb");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo tabela_hash.dat para busca");
        return;
    }

    TRegistro reg;
    int k = 0, pos;
    long long int colisoes = 0;

    while (k < TAMANHO_TABELA)
    {
        pos = hash(cpf, TAMANHO_TABELA, k);
        fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
        fread(&reg, sizeof(TRegistro), 1, arquivo);

        if (reg.cpf == cpf)
        {
            printf("\nRegistro encontrado na posicao %d (Colisoes: %lld):\n", pos, colisoes);
            printf("CPF: %lld | Nome: %s | Nota: %d\n", reg.cpf, reg.nome, reg.nota);
            fclose(arquivo);
            return;
        }
        else if (reg.cpf == 0) // Slot vazio encontrado, significa que o CPF não está na tabela
        {
            break;
        }

        colisoes++; // Incrementa a contagem de colisões se precisar sondar mais
        k++;
    }

    printf("CPF %lld nao encontrado na tabela hash (Colisoes: %lld).\n", cpf, colisoes);
    fclose(arquivo);
}

// Exclui um registro do arquivo da tabela hash (tabela_hash.dat)
void exclui_tabela_hash(long long cpf)
{
    FILE *arquivo = fopen("tabela_hash.dat", "rb+");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo tabela_hash.dat para exclusao");
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
            memset(&vazio, 0, sizeof(TRegistro)); // Sobrescreve com um registro vazio

            fseek(arquivo, pos * sizeof(TRegistro), SEEK_SET);
            fwrite(&vazio, sizeof(TRegistro), 1, arquivo);

            printf("Registro com CPF %lld removido da posicao %d na tabela hash.\n", cpf, pos);
            fclose(arquivo);
            return;
        }
        else if (reg.cpf == 0) // Slot vazio significa que o CPF nunca esteve aqui ou já foi excluído
        {
            break;
        }

        k++;
    }

    printf("CPF %lld nao encontrado na tabela hash para remocao.\n", cpf);
    fclose(arquivo);
}

// Imprime todos os registros não vazios no arquivo da tabela hash (tabela_hash.dat)
void imprimir_tabela_hash()
{
    FILE *arquivo = fopen("tabela_hash.dat", "rb");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo tabela_hash.dat para impressao");
        return;
    }

    TRegistro reg;
    int count = 0;
    printf("\n--- Conteudo da Tabela Hash (%s) ---\n", "tabela_hash.dat");
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        fseek(arquivo, i * sizeof(TRegistro), SEEK_SET);
        fread(&reg, sizeof(TRegistro), 1, arquivo);

        if (reg.cpf != 0) // Imprime apenas slots não vazios
        {
            printf("Posicao %d | CPF: %lld | Nome: %s | Nota: %d\n", i, reg.cpf, reg.nome, reg.nota);
            count++;
        }
    }

    printf("Total de registros ativos na tabela hash: %d\n", count);
    printf("--- Fim da Tabela Hash ---\n");
    fclose(arquivo);
}

int main()
{
    printf("Inicializando tabela_hash.dat...\n");
    inicializa_tabela_hash();
    printf("tabela_hash.dat inicializada.\n");

    printf("\nLendo registros de 'registros.dat' e inserindo em 'tabela_hash.dat'...\n");
    // Passa "registros.dat" como o nome do arquivo de origem
    le_registros_origem_e_insere_na_hash("registros.dat");
    printf("Processo de insercao concluido.\n");

    printf("\n--- Testando operacoes na tabela_hash.dat ---\n");

    // Estas são inserções de teste, além dos 10.000 registros do arquivo.
    insere_tabela_hash("Alice Silva", 11223344556LL, 95);
    insere_tabela_hash("Bob Johnson", 66778899001LL, 88);
    // Esta deve mostrar "CPF ja existe" se o CPF 11223344556 já tiver sido inserido (seja pelo arquivo ou manualmente)
    insere_tabela_hash("Charlie Brown", 11223344556LL, 70);

    busca_tabela_hash(66778899001LL);
    busca_tabela_hash(99999999999LL); // CPF que provavelmente não existe

    exclui_tabela_hash(11223344556LL);
    exclui_tabela_hash(99999999999LL); // Tenta excluir CPF que não existe

    imprimir_tabela_hash();

    return 0;
}