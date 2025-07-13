#include "TARVBP.h"
#include <stdint.h>

// Struct igual à do gera_registros.c
typedef struct
{
    char nome[50];
    long long int cpf;
    int nota;
} Registro;

// Converte os 9 primeiros dígitos do CPF (long long int) para string (com zeros à esquerda)
void cpf9_para_str(long long int cpf, char *dest)
{
    long long int nove = cpf / 100; // remove os 2 últimos dígitos
    sprintf(dest, "%09lld", nove);
}

// Converte Registro (do seu arquivo) para Dados (usado na árvore B+)
void Registro_para_Dados(const Registro *reg, Dados *d)
{
    cpf9_para_str(reg->cpf, d->cpf);
    memcpy(d->nome, reg->nome, 50);
    d->nota_final = reg->nota;
}

int main()
{
    FILE *f = fopen("../registros.dat", "rb");
    if (!f)
    {
        perror("Erro ao abrir registros.dat");
        return 1;
    }
    inic_arq();
    long long raiz = -1;
    Registro reg;
    Dados d;
    int i = 0;
    while (fread(&reg, sizeof(Registro), 1, f) == 1)
    {
        Registro_para_Dados(&reg, &d);
        raiz = TARVBP_insere(raiz, &d, 2, "ArqIdx.bin", "ArqDados.bin");
        char chave[13];
        cpf9_para_str(reg.cpf, chave);
        printf("Inserido: CPF chave: %s | raiz: %lld | idx: %d\n", chave, raiz, i + 1);
        if ((i + 1) % 1000 == 0)
            printf("Inseridos %d registros...\n", i + 1);
        i++;
    }
    fclose(f);
    printf("Todos os registros foram inseridos na árvore B+!\n");

    // Verificação: busca todos os 10000 CPFs na árvore B+
    f = fopen("../registros.dat", "rb");
    if (!f)
    {
        perror("Erro ao abrir registros.dat para verificação");
        return 1;
    }
    int encontrados = 0;
    i = 0;
    while (fread(&reg, sizeof(Registro), 1, f) == 1)
    {
        char chave[13];
        cpf9_para_str(reg.cpf, chave);
        long long pos = busca_cpf("ArqIdx.bin", chave, raiz);
        printf("Busca: CPF chave: %s | idx: %d | resultado: %lld\n", chave, i + 1, pos);
        if (pos != -1)
            encontrados++;
        else
            printf("Chave não encontrada: %s (registro %d)\n", chave, i + 1);
        i++;
    }
    fclose(f);
    printf("%d de %d registros encontrados na árvore B+ (busca por chave).\n", encontrados, i);
    return 0;
}
