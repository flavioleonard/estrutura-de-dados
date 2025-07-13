#include "TARVBP.h"
#include <stdio.h>
#include <string.h>

// Função para ler 11 dígitos do CPF do usuário e extrair os 9 primeiros como chave
void ler_cpf_chave(long long int *cpf, char *chave)
{
    char cpf_str[16];
    printf("Digite os 11 digitos do CPF: ");
    scanf("%15s", cpf_str);
    *cpf = atoll(cpf_str);
    strncpy(chave, cpf_str, 9);
    chave[9] = '\0';
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
        printf("\nMenu Arvore B+\n");
        printf("1 - Buscar registro pelo CPF (11 digitos)\n");
        printf("2 - Inserir novo registro\n");
        printf("3 - Remover registro pelo CPF (11 digitos)\n");
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
            long long int cpf;
            ler_cpf_chave(&cpf, chave);
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
                printf("CPF: %011lld\n", d.cpf); // Mostra os 11 dígitos
                printf("Nome: %s\n", d.nome);
                printf("Nota final: %d\n", d.nota_final);
            }
        }
        else if (op == 2)
        {
            Dados d;
            char chave9[10];
            ler_cpf_chave(&d.cpf, chave9);
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
            // Insere na árvore B+ usando só os 9 primeiros dígitos como chave
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
        else if (op == 3)
        {
            char chave9[10];
            long long int cpf;
            ler_cpf_chave(&cpf, chave9);
            int diminuiu = 0;
            raiz = TARVBP_remove(raiz, chave9, 2, "bplus_index.dat", "bplus_dados.dat", &diminuiu);
            // Atualiza o arquivo da raiz
            FILE *fraiz = fopen("raiz.dat", "wb");
            if (fraiz)
            {
                fwrite(&raiz, sizeof(long long), 1, fraiz);
                fclose(fraiz);
            }
            printf("Remoção concluída.\n");
        }
        else if (op != 0)
        {
            printf("Opcao invalida!\n");
        }
    } while (op != 0);
    printf("Saindo...\n");
    return 0;
}
