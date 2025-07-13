#include "TARVBP.h"
#include <stdio.h>
#include <string.h>

// Função para ler 9 dígitos do CPF do usuário
void ler_cpf_chave(char *chave)
{
    printf("Digite os 9 primeiros digitos do CPF (chave): ");
    scanf("%9s", chave);
    chave[9] = '\0';
    // Limpa o buffer até o fim da linha para evitar problemas se o usuário digitar mais de 9 dígitos
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main()
{
    long long raiz = -1;
    FILE *fidx = fopen("bplus_index.dat", "rb");
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
        printf("\n2Menu Arvore B+\n");
        printf("1 - Buscar registro pelo CPF (9 digitos)\n");
        printf("2 - Inserir novo registro\n");
        printf("0 - Sair\n");
        printf("\nEscolha: ");
        if (scanf("%d", &op) != 1)
        {
            while (getchar() != '\n')
                ; // Limpa buffer se entrada inválida
            op = -1;
        }
        else
        {
            while (getchar() != '\n')
                ; // Limpa buffer após scanf
        }

        if (op == 1)
        {
            ler_cpf_chave(chave);
            long long pos = busca_cpf("bplus_index.dat", chave, raiz);
            if (pos == -1)
            {
                printf("Registro nao encontrado para chave %s\n", chave);
            }
            else
            {
                FILE *fdados = fopen("bplus_dados.dat", "rb");
                if (!fdados)
                {
                    printf("Arquivo de dados nao encontrado!\n");
                    continue;
                }
                fseek(fdados, pos, SEEK_SET);
                Dados d;
                fread(&d, sizeof(Dados), 1, fdados);
                fclose(fdados);
                // Exibe o registro encontrado
                printf("\nRegistro encontrado:\n");
                printf("CPF: %s\n", d.cpf);
                printf("Nome: %s\n", d.nome);
                printf("Nota final: %d\n", d.nota_final);
            }
        }
        else if (op == 2)
        {
            Dados d;
            printf("Digite os 9 primeiros digitos do CPF (chave): ");
            scanf("%9s", d.cpf);
            d.cpf[9] = '\0';
            while (getchar() != '\n')
                ; // Limpa buffer
            printf("Digite o nome (ate 50 caracteres): ");
            fgets(d.nome, sizeof(d.nome), stdin);
            size_t len = strlen(d.nome);
            if (len > 0 && d.nome[len - 1] == '\n')
                d.nome[len - 1] = '\0';
            printf("Digite a nota (0 a 100): ");
            while (scanf("%d", &d.nota_final) != 1)
            {
                printf("Entrada invalida. Digite a nota (0 a 100): ");
                while (getchar() != '\n')
                    ;
            }
            while (getchar() != '\n')
                ; // Limpa buffer
            // Insere na árvore B+
            raiz = TARVBP_insere(raiz, &d, 2, "bplus_index.dat", "bplus_dados.dat");
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
            printf("Opcao invalida!\n");
        }
    } while (op != 0);
    printf("Saindo...\n");
    return 0;
}
