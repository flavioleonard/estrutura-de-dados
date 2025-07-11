#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM 500
#define MAX_CHAVES (2 * ORDEM)
#define MAX_FILHOS (2 * ORDEM + 1)
#define TAM_NOME 50
#define TAM_CPF 11
#define TOTAL_REGISTROS 10000

typedef struct
{
    char nome[TAM_NOME];
    char cpf[TAM_CPF];
    int nota;
} Registro;

// Estrutura do nó folha da B+
typedef struct BMaisFolha
{
    long int chaves[MAX_CHAVES];    // Chaves (9 primeiros dígitos do CPF)
    Registro registros[MAX_CHAVES]; // Dados completos
    int num_chaves;
    struct BMaisFolha *prox; // Ponteiro para próxima folha
} BMaisFolha;

// Estrutura do nó interno da B+
typedef struct BMaisNo
{
    long int chaves[MAX_CHAVES];
    void *filhos[MAX_FILHOS]; // Ponteiros para filhos (nós internos ou folhas)
    int num_chaves;
    int eh_folha;
} BMaisNo;


long int extrair_chave(char cpf[TAM_CPF])
{
    long int chave = 0;
    for (int i = 0; i < 9; i++)
    {
        chave = chave * 10 + (cpf[i] - '0');
    }
    return chave;
}


// Função para criar uma nova folha
BMaisFolha *criar_folha()
{
    BMaisFolha *folha = (BMaisFolha *)malloc(sizeof(BMaisFolha));
    if (!folha)
    {
        printf("[ERRO] Falha ao alocar memória para folha!\n");
        exit(1);
    }
    folha->num_chaves = 0;
    folha->prox = NULL;
    return folha;
}

// Função para criar um novo nó interno
BMaisNo *criar_no_interno()
{
    BMaisNo *no = (BMaisNo *)malloc(sizeof(BMaisNo));
    no->num_chaves = 0;
    no->eh_folha = 0;
    memset(no->filhos, 0, sizeof(no->filhos));
    return no;
}

// Estrutura da árvore B+
typedef struct
{
    void *raiz;
    int eh_folha_raiz;
} BMaisArvore;

// Função para inserir na folha (sem split)
void inserir_na_folha(BMaisFolha *folha, long int chave, Registro reg)
{
    int i = folha->num_chaves - 1;
    while (i >= 0 && folha->chaves[i] > chave)
    {
        folha->chaves[i + 1] = folha->chaves[i];
        folha->registros[i + 1] = folha->registros[i];
        i--;
    }
    folha->chaves[i + 1] = chave;
    folha->registros[i + 1] = reg;
    folha->num_chaves++;
}

// Função para split de folha
BMaisFolha *split_folha(BMaisFolha *folha, long int *chave_promovida)
{
    BMaisFolha *nova_folha = criar_folha();
    int meio = MAX_CHAVES / 2;
    int i;
    for (i = meio; i < folha->num_chaves; i++)
    {
        if (nova_folha->num_chaves >= MAX_CHAVES)
        {
            exit(1);
        }
        nova_folha->chaves[nova_folha->num_chaves] = folha->chaves[i];
        nova_folha->registros[nova_folha->num_chaves] = folha->registros[i];
        nova_folha->num_chaves++;
    }
    folha->num_chaves = meio;
    nova_folha->prox = folha->prox;
    folha->prox = nova_folha;
    *chave_promovida = nova_folha->chaves[0];
    return nova_folha;
}

// Função para split de nó interno
BMaisNo *split_no(BMaisNo *no, long int *chave_promovida)
{
    BMaisNo *novo_no = criar_no_interno();
    novo_no->eh_folha = no->eh_folha; // Corrige o campo eh_folha!
    int meio = MAX_CHAVES / 2;
    int i;
    // Copia metade superior para novo nó
    for (i = meio + 1; i < MAX_CHAVES; i++)
    {
        novo_no->chaves[novo_no->num_chaves] = no->chaves[i];
        novo_no->filhos[novo_no->num_chaves] = no->filhos[i];
        novo_no->num_chaves++;
    }
    novo_no->filhos[novo_no->num_chaves] = no->filhos[MAX_CHAVES];
    *chave_promovida = no->chaves[meio];
    no->num_chaves = meio;
    return novo_no;
}

// Função recursiva para inserir na árvore B+
void *inserir_rec(void *no, int eh_folha, long int chave, Registro reg, long int *chave_promovida, void **novo_filho)
{
    if (eh_folha)
    {
        BMaisFolha *folha = (BMaisFolha *)no;
        if (folha->num_chaves < MAX_CHAVES)
        {
            inserir_na_folha(folha, chave, reg);
            *novo_filho = NULL;
            return folha;
        }
        else
        {
            // Cria um array temporário para inserir o novo elemento e fazer o split corretamente
            long int temp_chaves[MAX_CHAVES + 1];
            Registro temp_regs[MAX_CHAVES + 1];
            int i = 0, j = 0;
            // Copia e insere na posição correta
            while (i < folha->num_chaves && folha->chaves[i] < chave)
            {
                temp_chaves[j] = folha->chaves[i];
                temp_regs[j] = folha->registros[i];
                i++;
                j++;
            }
            temp_chaves[j] = chave;
            temp_regs[j] = reg;
            j++;
            while (i < folha->num_chaves)
            {
                temp_chaves[j] = folha->chaves[i];
                temp_regs[j] = folha->registros[i];
                i++;
                j++;
            }
            // Atualiza folha original
            for (i = 0; i < MAX_CHAVES; i++)
            {
                folha->chaves[i] = temp_chaves[i];
                folha->registros[i] = temp_regs[i];
            }
            folha->num_chaves = MAX_CHAVES;
            // Split
            BMaisFolha *nova_folha = split_folha(folha, chave_promovida);
            // Copia o restante para nova folha
            for (i = 0; i < nova_folha->num_chaves; i++)
            {
                nova_folha->chaves[i] = temp_chaves[MAX_CHAVES / 2 + i];
                nova_folha->registros[i] = temp_regs[MAX_CHAVES / 2 + i];
            }
            *novo_filho = nova_folha;
            return folha;
        }
    }
    else
    {
        BMaisNo *interno = (BMaisNo *)no;
        int i = 0;
        while (i < interno->num_chaves && chave > interno->chaves[i])
            i++;
        long int chave_promovida_filho;
        void *novo_filho_filho = NULL;
        // Correção: todos os filhos são folhas se interno->eh_folha == 1
        int eh_folha_filho = interno->eh_folha;
        interno->filhos[i] = inserir_rec(interno->filhos[i], eh_folha_filho, chave, reg, &chave_promovida_filho, &novo_filho_filho);
        if (novo_filho_filho)
        {
            // Inserir chave e ponteiro no nó interno
            int j = interno->num_chaves;
            while (j > i)
            {
                interno->chaves[j] = interno->chaves[j - 1];
                interno->filhos[j + 1] = interno->filhos[j];
                j--;
            }
            interno->chaves[i] = chave_promovida_filho;
            interno->filhos[i + 1] = novo_filho_filho;
            interno->num_chaves++;
            if (interno->num_chaves > MAX_CHAVES)
            {
                BMaisNo *novo_no = split_no(interno, chave_promovida);
                *novo_filho = novo_no;
            }
            else
            {
                *novo_filho = NULL;
            }
        }
        else
        {
            *novo_filho = NULL;
        }
        return interno;
    }
}

// Função principal de inserção na árvore B+
void inserir_bmais(BMaisArvore *arv, long int chave, Registro reg)
{
    if (arv->raiz == NULL)
    {
        BMaisFolha *folha = criar_folha();
        inserir_na_folha(folha, chave, reg);
        arv->raiz = folha;
        arv->eh_folha_raiz = 1;
        return;
    }
    long int chave_promovida = 0;
    void *novo_filho = NULL;
    arv->raiz = inserir_rec(arv->raiz, arv->eh_folha_raiz, chave, reg, &chave_promovida, &novo_filho);
    if (novo_filho)
    {
        BMaisNo *nova_raiz = criar_no_interno();
        nova_raiz->chaves[0] = chave_promovida;
        nova_raiz->filhos[0] = arv->raiz;
        nova_raiz->filhos[1] = novo_filho;
        nova_raiz->num_chaves = 1;
        // Corrige: se a raiz anterior era folha, os filhos do novo nó são folhas!
        if (arv->eh_folha_raiz)
            nova_raiz->eh_folha = 1;
        else
            nova_raiz->eh_folha = 0;
        arv->raiz = nova_raiz;
        arv->eh_folha_raiz = 0;
    }
}

// Função para salvar as folhas (dados) em arquivo
void salvar_folhas(BMaisFolha *folha, const char *nome_arquivo)
{
    FILE *f = fopen(nome_arquivo, "wb");
    if (!f)
    {
        perror("Erro ao abrir arquivo de dados das folhas");
        return;
    }
    int folha_count = 0;
    while (folha)
    {
        if (folha->num_chaves < 0 || folha->num_chaves > MAX_CHAVES)
        {
            break;
        }
        if (folha->prox == folha)
        {
            break;
        }
        fwrite(folha->registros, sizeof(Registro), folha->num_chaves, f);
        folha = folha->prox;
        folha_count++;
    }
    fclose(f);
}

// Função recursiva para salvar o índice (nós internos e folhas) em arquivo
void salvar_indice_rec(void *no, int eh_folha, FILE *f)
{
    if (eh_folha)
    {
        BMaisFolha *folha = (BMaisFolha *)no;
        fwrite(&folha->num_chaves, sizeof(int), 1, f);
        fwrite(folha->chaves, sizeof(long int), folha->num_chaves, f);
        // Não salva os registros completos no índice
        if (folha->prox)
        {
            long int prox_primeira_chave = folha->prox->chaves[0];
            fwrite(&prox_primeira_chave, sizeof(long int), 1, f);
        }
        else
        {
            long int nulo = -1;
            fwrite(&nulo, sizeof(long int), 1, f);
        }
    }
    else
    {
        BMaisNo *interno = (BMaisNo *)no;
        fwrite(&interno->num_chaves, sizeof(int), 1, f);
        fwrite(interno->chaves, sizeof(long int), interno->num_chaves, f);
        for (int i = 0; i <= interno->num_chaves; i++)
        {
            // Correção: todos os filhos são folhas se interno->eh_folha == 1
            int eh_folha_filho = interno->eh_folha;
            salvar_indice_rec(interno->filhos[i], eh_folha_filho, f);
        }
    }
}

// Função para salvar o índice da árvore B+
void salvar_indice(BMaisArvore *arv, const char *nome_arquivo)
{
    FILE *f = fopen(nome_arquivo, "wb");
    if (!f)
    {
        perror("Erro ao abrir arquivo de índice");
        return;
    }
    salvar_indice_rec(arv->raiz, arv->eh_folha_raiz, f);
    fclose(f);
}

// Função de busca (consulta) na árvore B+
Registro *buscar_bmais(BMaisArvore *arv, long int chave)
{
    void *no = arv->raiz;
    int eh_folha = arv->eh_folha_raiz;
    while (!eh_folha)
    {
        BMaisNo *interno = (BMaisNo *)no;
        int i = 0;
        while (i < interno->num_chaves && chave > interno->chaves[i])
            i++;
        // Corrigido: se interno->eh_folha == 1, os filhos são folhas
        if (interno->eh_folha)
        {
            eh_folha = 1;
            no = interno->filhos[i];
        }
        else
        {
            BMaisNo *filho = (BMaisNo *)interno->filhos[i];
            eh_folha = filho->eh_folha;
            no = interno->filhos[i];
        }
    }
    BMaisFolha *folha = (BMaisFolha *)no;
    for (int i = 0; i < folha->num_chaves; i++)
    {
        if (folha->chaves[i] == chave)
        {
            return &folha->registros[i];
        }
    }
    return NULL;
}

// Função auxiliar para encontrar o índice da chave em um nó folha
int indice_chave_folha(BMaisFolha *folha, long int chave)
{
    for (int i = 0; i < folha->num_chaves; i++)
    {
        if (folha->chaves[i] == chave)
            return i;
    }
    return -1;
}

// Função para remover de folha
int remover_de_folha(BMaisFolha *folha, long int chave)
{
    int idx = indice_chave_folha(folha, chave);
    if (idx == -1)
        return 0; // Não encontrado
    for (int i = idx; i < folha->num_chaves - 1; i++)
    {
        folha->chaves[i] = folha->chaves[i + 1];
        folha->registros[i] = folha->registros[i + 1];
    }
    folha->num_chaves--;
    return 1;
}

// Função recursiva para remoção
int remover_rec(void *no, int eh_folha, long int chave, int *underflow)
{
    if (eh_folha)
    {
        BMaisFolha *folha = (BMaisFolha *)no;
        int ok = remover_de_folha(folha, chave);
        *underflow = (folha->num_chaves < ORDEM);
        return ok;
    }
    else
    {
        BMaisNo *interno = (BMaisNo *)no;
        int i = 0;
        while (i < interno->num_chaves && chave > interno->chaves[i])
            i++;
        int eh_folha_filho = ((BMaisNo *)interno->filhos[i])->eh_folha;
        int underflow_filho = 0;
        int ok = remover_rec(interno->filhos[i], eh_folha_filho, chave, &underflow_filho);
        if (underflow_filho)
        {
            // Redistribuição ou concatenação
            // Para simplificação, só concatena com irmão à direita se possível
            if (i < interno->num_chaves)
            {
                BMaisFolha *folha = (BMaisFolha *)interno->filhos[i];
                BMaisFolha *irmao = (BMaisFolha *)interno->filhos[i + 1];
                if (folha->num_chaves + irmao->num_chaves <= MAX_CHAVES)
                {
                    // Concatena
                    for (int j = 0; j < irmao->num_chaves; j++)
                    {
                        folha->chaves[folha->num_chaves] = irmao->chaves[j];
                        folha->registros[folha->num_chaves] = irmao->registros[j];
                        folha->num_chaves++;
                    }
                    folha->prox = irmao->prox;
                    // Remove ponteiro e chave do nó interno
                    for (int j = i; j < interno->num_chaves - 1; j++)
                    {
                        interno->chaves[j] = interno->chaves[j + 1];
                        interno->filhos[j + 1] = interno->filhos[j + 2];
                    }
                    interno->num_chaves--;
                    free(irmao);
                }
                else
                {
                    // Redistribuição: move um registro do irmão para folha
                    folha->chaves[folha->num_chaves] = irmao->chaves[0];
                    folha->registros[folha->num_chaves] = irmao->registros[0];
                    folha->num_chaves++;
                    for (int j = 0; j < irmao->num_chaves - 1; j++)
                    {
                        irmao->chaves[j] = irmao->chaves[j + 1];
                        irmao->registros[j] = irmao->registros[j + 1];
                    }
                    irmao->num_chaves--;
                    interno->chaves[i] = irmao->chaves[0];
                }
            }
        }
        *underflow = (interno->num_chaves < ORDEM);
        return ok;
    }
}

// Função principal de remoção
int remover_bmais(BMaisArvore *arv, long int chave)
{
    if (!arv->raiz)
        return 0;
    int underflow = 0;
    int ok = remover_rec(arv->raiz, arv->eh_folha_raiz, chave, &underflow);
    // Se a raiz ficou com 0 chaves e não é folha, desce um nível
    if (!arv->eh_folha_raiz && ((BMaisNo *)arv->raiz)->num_chaves == 0)
    {
        void *nova_raiz = ((BMaisNo *)arv->raiz)->filhos[0];
        free(arv->raiz);
        arv->raiz = nova_raiz;
        arv->eh_folha_raiz = ((BMaisNo *)nova_raiz)->eh_folha;
    }
    return ok;
}

// Função para imprimir todos os registros das folhas da árvore B+
// void imprimir_folhas(BMaisArvore *arv)
// {
//     if (!arv->raiz)
//     {
//         return;
//     }
//     BMaisFolha *folha = NULL;
//     if (arv->eh_folha_raiz)
//     {
//         folha = (BMaisFolha *)arv->raiz;
//     }
//     else
//     {
//         // Encontra a folha mais à esquerda
//         BMaisNo *no = (BMaisNo *)arv->raiz;
//         void *ptr = no;
//         while (!((BMaisNo *)ptr)->eh_folha)
//             ptr = ((BMaisNo *)ptr)->filhos[0];
//         folha = (BMaisFolha *)ptr;
//     }
//     int folha_num = 1;
//     while (folha)
//     {
//         for (int i = 0; i < folha->num_chaves; i++)
//         {
//             printf("Chave: %ld | Nome: %.*s | Nota: %d\n", folha->chaves[i], TAM_NOME, folha->registros[i].nome, folha->registros[i].nota);
//         }
//         folha = folha->prox;
//         folha_num++;
//     }
// }

// Nova função para imprimir o índice e o bloco de dados correspondente de cada folha
void imprimir_indices_e_blocos(BMaisArvore *arv)
{
    if (!arv->raiz)
    {
        printf("Árvore vazia.\n");
        return;
    }
    BMaisFolha *folha = NULL;
    if (arv->eh_folha_raiz)
    {
        folha = (BMaisFolha *)arv->raiz;
    }
    else
    {
        BMaisNo *no = (BMaisNo *)arv->raiz;
        void *ptr = no;
        while (!((BMaisNo *)ptr)->eh_folha)
            ptr = ((BMaisNo *)ptr)->filhos[0];
        folha = (BMaisFolha *)ptr;
    }
    int bloco = 0;
    while (folha)
    {
        printf("Bloco de dados %d: ", bloco);
        
        printf("\n");
        folha = folha->prox;
        bloco++;
    }
}

// Função para buscar e imprimir a folha e todos os registros de uma chave
void consulta_e_imprime_folha(BMaisArvore *arv, long int chave)
{
    void *no = arv->raiz;
    int eh_folha = arv->eh_folha_raiz;
    while (!eh_folha)
    {
        BMaisNo *interno = (BMaisNo *)no;
        int i = 0;
        while (i < interno->num_chaves && chave > interno->chaves[i])
            i++;
        if (interno->eh_folha)
        {
            eh_folha = 1;
            no = interno->filhos[i];
        }
        else
        {
            BMaisNo *filho = (BMaisNo *)interno->filhos[i];
            eh_folha = filho->eh_folha;
            no = interno->filhos[i];
        }
    }
    BMaisFolha *folha = (BMaisFolha *)no;
    int idx = -1;
    for (int i = 0; i < folha->num_chaves; i++)
    {
        if (folha->chaves[i] == chave)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        printf("Chave %ld não encontrada em nenhuma folha.\n", chave);
        return;
    }
    printf("Chave %ld encontrada na folha em %p.\n", chave, (void *)folha);
    printf("Todos os registros desta folha:\n");
    for (int i = 0; i < folha->num_chaves; i++)
    {
        printf("Chave: %ld | Nome: %.*s | Nota: %d\n", folha->chaves[i], TAM_NOME, folha->registros[i].nome, folha->registros[i].nota);
    }
}



int main()
{

    FILE *arq = fopen("registros.dat", "rb");
    if (!arq)
    {
        perror("Erro ao abrir registros.dat");
        return 1;
    }
    // Verifica tamanho do struct Registro
    fseek(arq, 0, SEEK_END);
    long int tam_arquivo = ftell(arq);
    fseek(arq, 0, SEEK_SET);
    if (tam_arquivo % sizeof(Registro) != 0)
    {
        printf("Erro: O arquivo registros.dat não está compatível com o struct Registro.\n");
        fclose(arq);
        return 1;
    }
    Registro reg;
    int count = 0;
    BMaisArvore arvore = {0};
    // Leitura e inserção na árvore B+
    while (fread(&reg, sizeof(Registro), 1, arq) == 1)
    {
        long int chave = extrair_chave(reg.cpf);
        inserir_bmais(&arvore, chave, reg);
        count++;
    }
    fclose(arq);
    if (count == 0)
    {
        printf("Nenhum registro foi lido. Verifique o arquivo registros.dat.\n");
        return 1;
    }
    if (arvore.eh_folha_raiz)
        salvar_folhas((BMaisFolha *)arvore.raiz, "bplus_dados.dat");
    else
    {
        BMaisNo *no = (BMaisNo *)arvore.raiz;
        void *folha = no;
        int debug_count = 0;
        while (!((BMaisNo *)folha)->eh_folha)
        {
            folha = ((BMaisNo *)folha)->filhos[0];
            debug_count++;
            if (folha == NULL)
            {
                return 1;
            }
        }
        salvar_folhas((BMaisFolha *)folha, "bplus_dados.dat");
    }
    
    salvar_indice(&arvore, "bplus_index.dat");

    // Imprime índice e bloco de dados de cada folha
    //imprimir_folhas(&arvore);

    // Exemplos de uso das novas funções:
    // Exemplo de consulta
    long int chave_busca = 249568221;
    consulta_e_imprime_folha(&arvore, chave_busca);

    /*
    // Exemplo de remoção
    long int chave_remover = 123456789;
    int ok = remover_bmais(&arvore, chave_remover);
    if (ok)
    {
        printf("Removido!\n");
    }
    else
    {
        printf("Chave não encontrada!\n");
    }
    */

    return 0;
}
