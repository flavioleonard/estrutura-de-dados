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
    FILE *fidx = fopen("ArqIdx.bin", "rb");
    if (!fidx)
    {
        printf("Arquivo de índice não encontrado. Gere a árvore primeiro!\n");
        return 1;
    }
    fclose(fidx);
    printf("Menu Árvore B+\n");
    printf("1 - Buscar registro pelo CPF (9 dígitos)\n");
    printf("0 - Sair\n");
    int op;
    char chave[13];
    do
    {
        printf("\nEscolha: ");
        scanf("%d", &op);
        getchar();
        if (op == 1)
        {
            ler_cpf_chave(chave);
            // A raiz precisa ser recuperada. Vamos assumir que é o primeiro nó do arquivo (offset 0)
            raiz = 0;
            long long pos = busca_cpf("ArqIdx.bin", chave, raiz);
            if (pos == -1)
            {
                printf("Registro não encontrado para chave %s\n", chave);
            }
            else
            {
                // Lê o registro correspondente em ArqDados.bin
                FILE *fdados = fopen("ArqDados.bin", "rb");
                if (!fdados)
                {
                    printf("Arquivo de dados não encontrado!\n");
                    continue;
                }
                fseek(fdados, pos, SEEK_SET);
                Dados d;
                fread(&d, sizeof(Dados), 1, fdados);
                fclose(fdados);
                printf("Registro encontrado:\n");
                printf("CPF: %s\n", d.cpf);
                printf("Nome: %.*s\n", 50, d.nome);
                printf("Nota: %d\n", d.nota_final);
            }
        }
        else if (op != 0)
        {
            printf("Opção inválida!\n");
        }
    } while (op != 0);
    printf("Saindo...\n");
    return 0;
}
