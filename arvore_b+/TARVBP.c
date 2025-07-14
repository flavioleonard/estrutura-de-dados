
#include "TARVBP.h"
#include <string.h>

static int remove_de_folha(TARVBP *no, const char *chave)
{
    int i = 0;
    while (i < no->num_chaves && strcmp(no->chaves[i], chave) < 0)
        i++;
    if (i == no->num_chaves || strcmp(no->chaves[i], chave) != 0)
        return 0; // não encontrou
    for (; i < no->num_chaves - 1; i++)
    {
        strcpy(no->chaves[i], no->chaves[i + 1]);
        no->reg[i] = no->reg[i + 1];
    }
    no->num_chaves--;
    return 1;
}

static void zera_registro_dados(const char *dados, long long pos)
{
    Dados vazio = {0};
    FILE *f = fopen(dados, "rb+");
    if (f)
    {
        fseek(f, pos, SEEK_SET);
        fwrite(&vazio, sizeof(Dados), 1, f);
        fclose(f);
    }
}

long long TARVBP_remove(long reg, char *chave, int t, char *idx, char *dados, int *diminuiu)
{
    if (reg == -1)
    {
        *diminuiu = 0;
        return -1;
    }
    TARVBP no;
    ler_no(idx, reg, &no);
    int i = 0;
    while (i < no.num_chaves && strcmp(chave, no.chaves[i]) > 0)
        i++;

    if (no.folha)
    {
        // Remover do arquivo de dados
        int found = 0;
        for (int j = 0; j < no.num_chaves; j++)
        {
            if (strcmp(no.chaves[j], chave) == 0)
            {
                zera_registro_dados(dados, no.reg[j]);
                found = 1;
                break;
            }
        }
        int ok = remove_de_folha(&no, chave);
        if (!ok)
        {
            *diminuiu = 0;
            return reg;
        }
        escrever_no(idx, reg, &no);
        printf("[DEBUG] Chaves do no folha apos remocao: ");
        for (int k = 0; k < no.num_chaves; k++)
        {
            printf("%s ", no.chaves[k]);
        }
        printf("\n");
        *diminuiu = (no.num_chaves < t - 1);
        return reg;
    }

    // Nó interno: desce recursivamente
    long long filho = no.filhos[i];
    int diminuiu_filho = 0;
    long long novo_filho = TARVBP_remove(filho, chave, t, idx, dados, &diminuiu_filho);
    no.filhos[i] = novo_filho;
    // TODO: Atualizar chaves do nó interno se necessário
    // TODO: Tratar underflow: redistribuição ou concatenação
    escrever_no(idx, reg, &no);
    *diminuiu = 0; // Atualizar conforme tratamento de underflow
    return reg;
}

#include "TARVBP.h"
#include <string.h>
int nos_atual = 0;

// Remove a chave de um nó folha
// ...existing code...

TARVBP TARVBP_cria(char *idx)
{
    TARVBP no;
    no.folha = 1;
    no.num_chaves = 0;
    int i = 0;
    for (; i < TAM * 2 - 1; i++)
    {
        strcpy(no.chaves[i], "-1");
        no.filhos[i] = -1;
        no.reg[i] = -1;
    }
    no.end = nos_atual * sizeof(TARVBP);
    no.filhos[i] = -1;
    no.proximo_no = -1; // -1 indica que não há próximo
    // Grava o nó recém-criado no arquivo de índice
    FILE *arq_i = fopen(idx, "rb+");
    if (!arq_i)
    {
        arq_i = fopen(idx, "wb+"); // cria se não existir
        if (!arq_i)
        {
            printf("[ERRO] TARVBP_cria: fopen falhou para %s\n", idx);
            exit(1);
        }
    }
    if (fseek(arq_i, no.end, SEEK_SET) != 0)
    {
        printf("[ERRO] TARVBP_cria: fseek falhou para %s, end=%lld\n", idx, no.end);
        fclose(arq_i);
        exit(1);
    }
    if (fwrite(&no, sizeof(TARVBP), 1, arq_i) != 1)
    {
        printf("[ERRO] TARVBP_cria: fwrite falhou para %s, end=%lld\n", idx, no.end);
        fclose(arq_i);
        exit(1);
    }
    fclose(arq_i);
    return no;
}

void ler_no(char *arq, long long reg, TARVBP *no)
{
    FILE *arq_i = fopen(arq, "rb");
    if (!arq_i)
    {
        printf("[ERRO] ler_no: fopen falhou para %s\n", arq);
        exit(1);
    }
    if (fseek(arq_i, reg, SEEK_SET) != 0)
    {
        printf("[ERRO] ler_no: fseek falhou para %s, reg=%lld\n", arq, reg);
        fclose(arq_i);
        exit(1);
    }
    if (fread(no, sizeof(TARVBP), 1, arq_i) != 1)
    {
        printf("[ERRO] ler_no: fread falhou para %s, reg=%lld\n", arq, reg);
        fclose(arq_i);
        exit(1);
    }
    fclose(arq_i);
}

long long busca_cpf(char *arq_idx, char *cpf, long long reg)
{
    if (!arq_idx)
        return -1;

    long long reg_atual = reg;
    TARVBP no_atual;

    while (reg_atual != -1)
    {
        ler_no(arq_idx, reg_atual, &no_atual);

        if (no_atual.folha)
        {
            for (int i = 0; i < no_atual.num_chaves; i++)
            {
                if (!strcmp(no_atual.chaves[i], cpf))
                {
                    return no_atual.reg[i];
                }
            }
            return -1;
        }
        int i = 0;
        while (i < no_atual.num_chaves && strcmp(no_atual.chaves[i], cpf) <= 0)
            i++;
        reg_atual = no_atual.filhos[i];
    }
    return -1;
}

void escrever_no(char *arq, long long reg, TARVBP *no)
{
    FILE *arq_i = fopen(arq, "rb+");
    if (!arq_i)
    {
        printf("[ERRO] escrever_no: fopen falhou para %s\n", arq);
        exit(1);
    }
    if (fseek(arq_i, reg, SEEK_SET) != 0)
    {
        printf("[ERRO] escrever_no: fseek falhou para %s, reg=%lld\n", arq, reg);
        fclose(arq_i);
        exit(1);
    }
    if (fwrite(no, sizeof(TARVBP), 1, arq_i) != 1)
    {
        printf("[ERRO] escrever_no: fwrite falhou para %s, reg=%lld\n", arq, reg);
        fclose(arq_i);
        exit(1);
    }
    fclose(arq_i);
}

TARVBP divisao(TARVBP a, TARVBP p, int t, int i, char *idx)
{
    nos_atual++;
    TARVBP z = TARVBP_cria(idx);
    z.folha = a.folha;
    if (!a.folha)
    {
        z.num_chaves = t - 1;
        for (int j = 0; j < t - 1; j++)
        {
            strcpy(z.chaves[j], a.chaves[j + t]);
        }
        for (int k = 0; k < t; k++)
        {
            z.filhos[k] = a.filhos[t + k];
            a.filhos[t + k] = -1;
        }
    }
    else
    {
        z.num_chaves = t;
        for (int j = 0; j < t; j++)
        {
            z.reg[j] = a.reg[j + t - 1];
            strcpy(z.chaves[j], a.chaves[j + t - 1]);
        }
        z.proximo_no = a.proximo_no;
        a.proximo_no = z.end;
    }
    for (int j = p.num_chaves; j > i - 1; j--)
    {
        p.filhos[j + 1] = p.filhos[j];
    }
    p.filhos[i] = z.end;
    for (int k = p.num_chaves; k > i - 1; k--)
    {
        strcpy(p.chaves[k], p.chaves[k - 1]);
    }
    strcpy(p.chaves[i - 1], a.chaves[t - 1]);
    a.num_chaves = t - 1;
    p.num_chaves++;
    for (int jk = t - 1; jk < (2 * t) - 1; jk++)
    {
        a.reg[jk] = -1;
        strcpy(a.chaves[jk], "-1");
    }
    escrever_no(idx, a.end, &a);
    escrever_no(idx, z.end, &z);
    escrever_no(idx, p.end, &p);
    return p;
}

TARVBP insere_n_completo(TARVBP a, Dados *d, int t, char *idx, char *dados)
{
    int i;
    if (a.folha)
    { // finalmente insere de fato
        i = a.num_chaves - 1;
        // Gera a chave dos 9 primeiros dígitos do CPF
        char chave9[10];
        sprintf(chave9, "%09lld", d->cpf / 100);
        while ((i > -1) && (strcmp(chave9, a.chaves[i]) < 0))
        {
            strcpy(a.chaves[i + 1], a.chaves[i]);
            a.reg[i + 1] = a.reg[i];
            i--;
        }
        strcpy(a.chaves[i + 1], chave9);
        a.num_chaves++;
        FILE *arq_dados = fopen(dados, "rb+");
        fseek(arq_dados, 0, SEEK_END);
        a.reg[i + 1] = ftell(arq_dados);
        fwrite(d, sizeof(Dados), 1, arq_dados);
        escrever_no(idx, a.end, &a);
        fclose(arq_dados);
        return a;
    }
    i = a.num_chaves - 1;
    // Gera a chave dos 9 primeiros dígitos do CPF
    char chave9[10];
    sprintf(chave9, "%09lld", d->cpf / 100);
    while ((i > -1) && (strcmp(chave9, a.chaves[i]) < 0))
    {
        i--;
    }
    TARVBP f;
    ler_no(idx, a.filhos[i + 1], &f);
    i++;
    int div = 0;
    if (f.num_chaves == ((2 * t) - 1))
    { // no cheio
        a = divisao(f, a, t, i + 1, idx);
        div = 1;
        // Atualiza chave9 após possível divisão
        sprintf(chave9, "%09lld", d->cpf / 100);
        if (strcmp(a.chaves[i], chave9) < 0)
            i++;
    }
    if (div)
    { // divisao foi feita
        ler_no(idx, a.filhos[i], &f);
    }
    f = insere_n_completo(f, d, t, idx, dados);
    escrever_no(idx, f.end, &f);
    return a;
}

long long TARVBP_insere(long reg, Dados *d, int t, char *idx, char *dados)
{
    TARVBP a;
    if (reg != -1)
        ler_no(idx, reg, &a);
    else
        a = TARVBP_cria(idx);
    // Gera a chave dos 9 primeiros dígitos do CPF
    char chave9[10];
    sprintf(chave9, "%09lld", d->cpf / 100);
    if (busca_cpf(idx, chave9, a.end) != -1)
        return a.end;
    if (a.num_chaves != (2 * t) - 1)
    {
        a = insere_n_completo(a, d, t, idx, dados);
        return a.end;
    }
    nos_atual++;
    TARVBP p = TARVBP_cria(idx);
    p.filhos[0] = a.end;
    p.folha = 0;
    p = divisao(a, p, t, 1, idx);
    p = insere_n_completo(p, d, t, idx, dados);
    return p.end;
}
