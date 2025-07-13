// Árvore B+ em disco para registros de alunos
// Implementação baseada nas orientações do professor e no formato de registros.dat

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ORDEM 100 // d (ajuste conforme desejado para testes, ex: 50)
#define MAX_CHAVES (2 * ORDEM)
#define MAX_FILHOS (2 * ORDEM + 1)
#define TAM_NOME 50
#define TOTAL_REGISTROS 10000

// Estrutura do registro (igual ao gera_registros.c)
typedef struct
{
    char nome[TAM_NOME];
    long long int cpf; // 11 dígitos, mas só os 9 primeiros são chave
    int nota;
} Registro;

// Função para extrair a chave (9 primeiros dígitos do CPF)
long int extrair_chave(long long int cpf)
{
    return cpf / 100; // Remove os dois últimos dígitos
}

// Estrutura do nó folha (arquivo de dados)
typedef struct
{
    int num_registros; // m
    long int pai;      // offset do nó pai no arquivo de índice
    long int prox;     // offset da próxima folha no arquivo de dados
    Registro registros[MAX_CHAVES];
} NoFolha;

// Estrutura do nó interno (arquivo de índice)
typedef struct
{
    int num_chaves;              // m
    bool eh_folha;               // TRUE se aponta para folhas
    long int pai;                // offset do nó pai no arquivo de índice
    long int filhos[MAX_FILHOS]; // offsets dos filhos (índice ou dados)
    long int chaves[MAX_CHAVES]; // chaves (9 primeiros dígitos do CPF)
} NoIndice;

// Estrutura do arquivo de metadados
typedef struct
{
    long int raiz_offset; // offset da raiz no arquivo de índice
    bool raiz_eh_folha;   // TRUE se raiz é folha
} Metadados;

// Prototipação das funções principais
void criar_arvore_bmais(const char *arquivo_registros, const char *arquivo_indices, const char *arquivo_dados, const char *arquivo_meta);
void inserir_bplus(FILE *fidx, FILE *fdad, FILE *fmeta, Metadados *meta, Registro reg);

// Função principal
enum
{
    ARQ_OK = 0,
    ARQ_ERR = 1
};

int main()
{
    criar_arvore_bmais("registros.dat", "bplus_index.dat", "bplus_dados.dat", "bplus_meta.dat");
    printf("Árvore B+ criada e arquivos gerados!\n");
    return 0;
}

// Função para criar a árvore B+ lendo registros.dat e salvando arquivos de índice, dados e metadados
void criar_arvore_bmais(const char *arquivo_registros, const char *arquivo_indices, const char *arquivo_dados, const char *arquivo_meta)
{
    // 1. Abrir arquivos
    FILE *freg = fopen(arquivo_registros, "rb");
    if (!freg)
    {
        perror("Erro ao abrir registros.dat");
        exit(1);
    }
    else
    {
        printf("[DEBUG] registros.dat aberto com sucesso!\n");
    }
    FILE *fidx = fopen(arquivo_indices, "w+b");
    if (!fidx)
    {
        perror("Erro ao criar arquivo de índice");
        fclose(freg);
        exit(1);
    }
    FILE *fdad = fopen(arquivo_dados, "w+b");
    if (!fdad)
    {
        perror("Erro ao criar arquivo de dados");
        fclose(freg);
        fclose(fidx);
        exit(1);
    }
    FILE *fmeta = fopen(arquivo_meta, "w+b");
    if (!fmeta)
    {
        perror("Erro ao criar arquivo de metadados");
        fclose(freg);
        fclose(fidx);
        fclose(fdad);
        exit(1);
    }

    // 2. Inicializar metadados (raiz ainda não existe)
    Metadados meta = {.raiz_offset = -1, .raiz_eh_folha = true};
    fwrite(&meta, sizeof(Metadados), 1, fmeta);
    fflush(fmeta);

    // 3. Inicializar árvore em memória (apenas uma folha inicialmente)
    NoFolha folha_raiz;
    folha_raiz.num_registros = 0;
    folha_raiz.pai = -1;
    folha_raiz.prox = -1;
    // Offset da folha raiz no arquivo de dados
    long int folha_offset = ftell(fdad);
    fwrite(&folha_raiz, sizeof(NoFolha), 1, fdad);
    fflush(fdad);

    // Atualiza metadados para apontar para a folha raiz
    meta.raiz_offset = folha_offset;
    meta.raiz_eh_folha = true;
    rewind(fmeta);
    fwrite(&meta, sizeof(Metadados), 1, fmeta);
    fflush(fmeta);

    // 4. Ler registros.dat e inserir recursivamente na árvore B+
    Registro reg;
    int total = 0;
    while (fread(&reg, sizeof(Registro), 1, freg) == 1)
    {
        printf("[DEBUG] Lendo registro: CPF=%lld, Nome=%s, Nota=%d\n", reg.cpf, reg.nome, reg.nota);
        inserir_bplus(fidx, fdad, fmeta, &meta, reg);
        total++;
    }
    printf("Registros inseridos: %d\n", total);

    // Fecha arquivos
    fclose(freg);
    fclose(fidx);
    fclose(fdad);
    fclose(fmeta);
    printf("[INFO] Estrutura inicial criada. Split, índices e redistribuição ainda não implementados.\n");
}

// Outras funções auxiliares e de manipulação de nós, splits, concatenação, redistribuição, escrita/leitura em disco
// devem ser implementadas abaixo...
// Função auxiliar: escreve folha no arquivo de dados e retorna offset
long int escrever_folha(FILE *fdad, NoFolha *folha, long int offset)
{
    fseek(fdad, offset, SEEK_SET);
    fwrite(folha, sizeof(NoFolha), 1, fdad);
    fflush(fdad);
    return offset;
}

// Função auxiliar: escreve nó índice no arquivo de índices e retorna offset
long int escrever_indice(FILE *fidx, NoIndice *indice, long int offset)
{
    fseek(fidx, offset, SEEK_SET);
    fwrite(indice, sizeof(NoIndice), 1, fidx);
    fflush(fidx);
    return offset;
}

// Função auxiliar: lê folha do arquivo de dados
void ler_folha(FILE *fdad, NoFolha *folha, long int offset)
{
    fseek(fdad, offset, SEEK_SET);
    fread(folha, sizeof(NoFolha), 1, fdad);
}

// Função auxiliar: lê nó índice do arquivo de índices
void ler_indice(FILE *fidx, NoIndice *indice, long int offset)
{
    fseek(fidx, offset, SEEK_SET);
    fread(indice, sizeof(NoIndice), 1, fidx);
}

// Split de folha: retorna offset da nova folha criada e chave promovida
void split_folha(FILE *fdad, NoFolha *folha, long int folha_offset, long int *nova_folha_offset, long int *chave_promovida)
{
    NoFolha nova_folha;
    nova_folha.num_registros = 0;
    nova_folha.pai = folha->pai;
    nova_folha.prox = folha->prox;
    int meio = MAX_CHAVES / 2;
    for (int i = meio; i < MAX_CHAVES; i++)
    {
        nova_folha.registros[nova_folha.num_registros++] = folha->registros[i];
    }
    folha->num_registros = meio;
    // Atualiza ponteiros
    *nova_folha_offset = ftell(fdad);
    nova_folha.prox = folha->prox;
    folha->prox = *nova_folha_offset;
    // Escreve folhas
    escrever_folha(fdad, folha, folha_offset);
    escrever_folha(fdad, &nova_folha, *nova_folha_offset);
    // Chave promovida: menor chave da nova folha
    *chave_promovida = extrair_chave(nova_folha.registros[0].cpf);
}

// Split de índice: retorna offset do novo índice criado e chave promovida
void split_indice(FILE *fidx, NoIndice *indice, long int indice_offset, long int *novo_indice_offset, long int *chave_promovida)
{
    NoIndice novo_indice;
    novo_indice.num_chaves = 0;
    novo_indice.eh_folha = indice->eh_folha;
    novo_indice.pai = indice->pai;
    int meio = MAX_CHAVES / 2;
    // Chave promovida é a do meio
    *chave_promovida = indice->chaves[meio];
    // Copia metade superior para novo índice
    int j = 0;
    for (int i = meio + 1; i < indice->num_chaves; i++)
    {
        novo_indice.chaves[j] = indice->chaves[i];
        novo_indice.filhos[j] = indice->filhos[i];
        j++;
        novo_indice.num_chaves++;
    }
    novo_indice.filhos[j] = indice->filhos[indice->num_chaves];
    // Atualiza índice original
    indice->num_chaves = meio;
    // Escreve índices
    *novo_indice_offset = ftell(fidx);
    escrever_indice(fidx, indice, indice_offset);
    escrever_indice(fidx, &novo_indice, *novo_indice_offset);
}

// Inserção recursiva: insere registro e propaga splits
void inserir_recursivo(FILE *fidx, FILE *fdad, FILE *fmeta, Metadados *meta, long int no_offset, Registro reg, bool eh_folha, long int *nova_chave, long int *novo_filho_offset)
{
    if (eh_folha)
    {
        // Inserir em folha
        NoFolha folha;
        ler_folha(fdad, &folha, no_offset);
        // Inserção ordenada
        int i = folha.num_registros - 1;
        while (i >= 0 && extrair_chave(folha.registros[i].cpf) > extrair_chave(reg.cpf))
        {
            folha.registros[i + 1] = folha.registros[i];
            i--;
        }
        folha.registros[i + 1] = reg;
        folha.num_registros++;
        if (folha.num_registros <= MAX_CHAVES)
        {
            if (nova_chave)
                *nova_chave = -1;
            if (novo_filho_offset)
                *novo_filho_offset = -1;
            escrever_folha(fdad, &folha, no_offset);
            return;
        }
        else
        {
            // Split de folha
            long int nova_folha_offset, chave_promovida;
            split_folha(fdad, &folha, no_offset, &nova_folha_offset, &chave_promovida);
            if (nova_chave)
                *nova_chave = chave_promovida;
            if (novo_filho_offset)
                *novo_filho_offset = nova_folha_offset;
            return;
        }
    }
    else
    {
        // Inserir em índice
        NoIndice indice;
        ler_indice(fidx, &indice, no_offset);
        // Descobre filho correto
        int pos = 0;
        while (pos < indice.num_chaves && extrair_chave(reg.cpf) >= indice.chaves[pos])
            pos++;
        long int nova_chave_val = -1, novo_filho_offset_val = -1;
        inserir_recursivo(fidx, fdad, fmeta, meta, indice.filhos[pos], reg, indice.eh_folha, &nova_chave_val, &novo_filho_offset_val);
        if (nova_chave_val == -1)
            return; // Não houve split abaixo
        // Inserir nova_chave e novo_filho_offset neste índice
        for (int i = indice.num_chaves; i > pos; i--)
        {
            indice.chaves[i] = indice.chaves[i - 1];
            indice.filhos[i + 1] = indice.filhos[i];
        }
        indice.chaves[pos] = nova_chave_val;
        indice.filhos[pos + 1] = novo_filho_offset_val;
        indice.num_chaves++;
        if (indice.num_chaves <= MAX_CHAVES)
        {
            escrever_indice(fidx, &indice, no_offset);
            return;
        }
        else
        {
            // Split de índice
            long int novo_indice_offset, chave_promovida;
            split_indice(fidx, &indice, no_offset, &novo_indice_offset, &chave_promovida);
            // Se este nó é a raiz, criar nova raiz
            if (indice.pai == -1)
            {
                NoIndice nova_raiz;
                nova_raiz.num_chaves = 1;
                nova_raiz.eh_folha = false;
                nova_raiz.pai = -1;
                nova_raiz.chaves[0] = chave_promovida;
                nova_raiz.filhos[0] = no_offset;
                nova_raiz.filhos[1] = novo_indice_offset;
                long int nova_raiz_offset = ftell(fidx);
                escrever_indice(fidx, &nova_raiz, nova_raiz_offset);
                // Atualiza metadados
                meta->raiz_offset = nova_raiz_offset;
                meta->raiz_eh_folha = false;
                rewind(fmeta);
                fwrite(meta, sizeof(Metadados), 1, fmeta);
                fflush(fmeta);
            }
            else
            {
                // Propaga split para cima
                if (nova_chave)
                    *nova_chave = chave_promovida;
                if (novo_filho_offset)
                    *novo_filho_offset = novo_indice_offset;
            }
        }
    }
}

// Nova função principal de inserção
void inserir_bplus(FILE *fidx, FILE *fdad, FILE *fmeta, Metadados *meta, Registro reg)
{
    long int nova_chave = -1, novo_filho_offset = -1;
    inserir_recursivo(fidx, fdad, fmeta, meta, meta->raiz_offset, reg, meta->raiz_eh_folha, &nova_chave, &novo_filho_offset);
    // Se houve split na raiz folha, criar nova raiz
    if (nova_chave != -1 && meta->raiz_eh_folha)
    {
        NoIndice nova_raiz;
        nova_raiz.num_chaves = 1;
        nova_raiz.eh_folha = true;
        nova_raiz.pai = -1;
        nova_raiz.chaves[0] = nova_chave;
        nova_raiz.filhos[0] = meta->raiz_offset;
        nova_raiz.filhos[1] = novo_filho_offset;
        long int nova_raiz_offset = ftell(fidx);
        escrever_indice(fidx, &nova_raiz, nova_raiz_offset);
        // Atualiza metadados
        meta->raiz_offset = nova_raiz_offset;
        meta->raiz_eh_folha = false;
        rewind(fmeta);
        fwrite(meta, sizeof(Metadados), 1, fmeta);
        fflush(fmeta);
    }
}
