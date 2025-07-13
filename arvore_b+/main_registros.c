#include "TARVBP.h"
#include <stdint.h>
#include <string.h>

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
    d->cpf = reg->cpf; // Mantém o CPF inteiro (11 dígitos)
    memcpy(d->nome, reg->nome, 50);
    d->nota_final = reg->nota;
}

int main()
{
    const char *caminho = "../registros.dat";
    printf("Tentando abrir o arquivo: %s\n", caminho);
    inic_arq();
    printf("Depois de inic_arq()\n");
    FILE *f = fopen(caminho, "rb");
    if (!f)
    {
        perror("Erro ao abrir registros.dat");
        printf("errno: %d\n", errno);
        return 1;
    }
    long long raiz = -1;
    Registro reg;
    Dados d;
    int i = 0;

    printf("Iniciando loop de insercao...\n");
    int inseridos = 0;
    while (fread(&reg, sizeof(Registro), 1, f) == 1)
    {
        Registro_para_Dados(&reg, &d);
        raiz = TARVBP_insere(raiz, &d, 2, "bplus_index.dat", "bplus_dados.dat");
        char chave[13];
        cpf9_para_str(reg.cpf, chave);
        printf("Inserido: CPF chave: %s | raiz: %lld | idx: %d\n", chave, raiz, inseridos + 1);
        inseridos++;
    }
    printf("Loop de insercao finalizado. Total inseridos: %d\n", inseridos);
    fclose(f);
    printf("Todos os registros foram inseridos na árvore B+!\n");

    // Verificação: busca todos os 10000 CPFs na árvore B+
    printf("Abrindo arquivo para verificação...\n");
    f = fopen("../registros.dat", "rb");
    if (!f)
    {
        perror("Erro ao abrir registros.dat para verificação");
        printf("errno: %d\n", errno);
        return 1;
    }
    printf("Arquivo aberto para verificação.\n");
    int encontrados = 0;
    i = 0;
    while (fread(&reg, sizeof(Registro), 1, f) == 1)
    {
        char chave[13];
        cpf9_para_str(reg.cpf, chave);
        long long pos = busca_cpf("bplus_index.dat", chave, raiz);
        printf("Busca: CPF chave: %s | idx: %d | resultado: %lld\n", chave, i + 1, pos);
        if (pos != -1)
            encontrados++;
        else
            printf("Chave nao encontrada: %s (registro %d)\n", chave, i + 1);
        i++;
    }
    printf("Loop de verificacao finalizado. Total verificados: %d\n", i);
    fclose(f);
    printf("%d de %d registros encontrados na árvore B+ (busca por chave).\n", encontrados, i);

    // Salva o valor da raiz para uso posterior
    FILE *fraiz = fopen("raiz.dat", "wb");
    if (fraiz)
    {
        fwrite(&raiz, sizeof(long long), 1, fraiz);
        fclose(fraiz);
    }
    else
    {
        printf("Aviso: não foi possível salvar o valor da raiz em raiz.dat!\n");
    }
    return 0;
}
