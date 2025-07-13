#include "TARVBP.h"
#include <stdio.h>
#include <string.h>

// Função para ler 9 dígitos do CPF do usuário
void ler_cpf_chave(char *chave)
{
    printf("Digite os 9 primeiros dígitos do CPF (chave): ");
    scanf("%9s", chave);
    chave[9] = '\0';
}

int main()
{
    long long raiz = -1;
    FILE *fidx = fopen("ArqIdx.dat", "rb");
    if (!fidx)
    {
        printf("Arquivo de índice não encontrado. Gere a árvore primeiro!\n");
        return 1;
    }
    fclose(fidx);
    FILE *fraiz = fopen("raiz.dat", "rb");
    if (!fraiz)
    {
        printf("Arquivo da raiz não encontrado. Gere a árvore primeiro!\n");
        return 1;
    }
    fread(&raiz, sizeof(long long), 1, fraiz);
    fclose(fraiz);
    int op;
    char chave[13];
    do
    {
        printf("Menu Árvore B+\n");
        printf("1 - Buscar registro pelo CPF (9 dígitos)\n");
        printf("2 - Inserir novo registro\n");
        printf("0 - Sair\n");
        printf("\nEscolha: ");
        scanf("%d", &op);
        getchar();
        if (op == 1)
        {
            ler_cpf_chave(chave);
            long long pos = busca_cpf("ArqIdx.dat", chave, raiz);
            if (pos == -1)
            {
                printf("Registro não encontrado para chave %s\n", chave);
            }
            else
            {
                FILE *fdados = fopen("ArqDados.dat", "rb");
                if (!fdados)
                {
                    printf("Arquivo de dados não encontrado!\n");
                    continue;
                }
                fseek(fdados, pos, SEEK_SET);
                Dados d;
                fread(&d, sizeof(Dados), 1, fdados);
                fclose(fdados);
            }
        }
        else if (op == 2)
        {
            Dados d;
            printf("Digite os 9 primeiros dígitos do CPF (chave): ");
            scanf("%9s", d.cpf);
            d.cpf[9] = '\0';
            while (getchar() != '\n')
                ; // Limpa buffer
            printf("Digite o nome (até 50 caracteres): ");
            fgets(d.nome, 50, stdin);
            size_t len = strlen(d.nome);
            if (len > 0 && d.nome[len - 1] == '\n')
                d.nome[len - 1] = '\0';
            printf("Digite a nota (0 a 100): ");
            scanf("%d", &d.nota_final);
            while (getchar() != '\n')
                ; // Limpa buffer
            // Insere na árvore B+
            raiz = TARVBP_insere(raiz, &d, 2, "ArqIdx.dat", "ArqDados.dat");
            // Atualiza o arquivo da raiz
            FILE *fraiz = fopen("raiz.dat", "wb");
            if (fraiz)
            {
                fwrite(&raiz, sizeof(long long), 1, fraiz);
                fclose(fraiz);
            }
            printf("Registro inserido com sucesso!\n");
        }
        else if (op != 0)
        {
            printf("Opção inválida!\n");
        }
    } while (op != 0);
    printf("Saindo...\n");
    return 0;
}
