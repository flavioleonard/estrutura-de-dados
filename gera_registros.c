#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TOTAL_REGISTROS 10000
#define TAM_NOME 50
#define TAM_CPF 11

typedef struct
{
    char nome[TAM_NOME];
    char cpf[TAM_CPF]; // CPF como string de 11 dígitos
    int nota;
} Registro;

const char *nomes[100] = {
    "Joao", "Pedro", "Lucas", "Mateus", "Gabriel", "Felipe", "Rafael", "Daniel", "Marcos", "Carlos",
    "Eduardo", "Andre", "Fernando", "Paulo", "Thiago", "Marcelo", "Rodrigo", "Bruno", "Alex", "Leonardo",
    "Ricardo", "Antonio", "Roberto", "Gustavo", "Diego", "Vinicius", "Renato", "Juliano", "Sergio", "Luiz",
    "Fabio", "Marcio", "Cesar", "Jose", "Mauricio", "Alberto", "Adriano", "William", "Rogerio", "Davi",
    "Arthur", "Miguel", "Bernardo", "Heitor", "Enzo", "Theo", "Oliver", "Benjamin", "Nicolas", "Guilherme",
    "Samuel", "Henry", "Dylan", "Bryan", "Victor", "Kevin", "Levi", "Ian", "Erick", "Nathan",
    "Caio", "Lucca", "Enrico", "Otavio", "Augusto", "Vitor", "Francisco", "Breno", "Igor", "Matheus",
    "Caua", "Ryan", "Joao Pedro", "Joao Gabriel", "Joao Lucas", "Joao Vitor", "Joao Miguel", "Joao Paulo", "Joao Marcos", "Joao Carlos",
    "Luan", "Kaique", "Emanuel", "Danilo", "Anderson", "Alan", "Alexandre", "Alessandro", "Claudio", "Cristiano",
    "Edson", "Everton", "Felipe", "Giovanni", "Hugo", "Ivan", "Jorge", "Julio", "Luciano", "Mario"};

const char *sobrenomes[100] = {
    "Silva", "Santos", "Oliveira", "Souza", "Rodrigues", "Ferreira", "Alves", "Pereira", "Lima", "Gomes",
    "Costa", "Ribeiro", "Martins", "Carvalho", "Araujo", "Melo", "Barbosa", "Monteiro", "Cardoso", "Teixeira",
    "Nascimento", "Dias", "Fernandes", "Rocha", "Moreira", "Correia", "Cunha", "Mendes", "Azevedo", "Medeiros",
    "Freitas", "Cavalcanti", "Vieira", "Pinto", "Machado", "Borges", "Andrade", "Campos", "Tavares", "Brito",
    "Vasconcelos", "Siqueira", "Queiroz", "Aguiar", "Diniz", "Fonseca", "Sales", "Peixoto", "Guedes", "Castro",
    "Pires", "Barros", "Franco", "Xavier", "Farias", "Leal", "Sampaio", "Paiva", "Cordeiro", "Baptista",
    "Maia", "Amaral", "Bezerra", "Dantas", "Galvao", "Barreto", "Veloso", "Lacerda", "Maciel", "Porto",
    "Rangel", "Viana", "Assis", "Valenca", "Coutinho", "Figueiredo", "Moraes", "Lobato", "Quintana", "Ramalho",
    "Salgado", "Holanda", "Albuquerque", "Flores", "Marques", "Reis", "Torres", "Caldeira", "Fagundes", "Miranda",
    "Pinheiro", "Furtado", "Bandeira", "Cabral", "Cerqueira", "Coelho", "Cruz", "Duarte", "Esteves", "Fontes"};

// Preenche o nome com espaços até completar 50 caracteres
void gerar_nome_completo(char *nome_completo)
{
    int idx_nome = rand() % 100;
    int idx_sobrenome = rand() % 100;
    char temp[2 * TAM_NOME];
    snprintf(temp, sizeof(temp), "%s %s", nomes[idx_nome], sobrenomes[idx_sobrenome]);
    size_t len = strlen(temp);
    if (len > TAM_NOME)
        len = TAM_NOME;
    memcpy(nome_completo, temp, len);
    if (len < TAM_NOME)
    {
        memset(nome_completo + len, ' ', TAM_NOME - len);
    }
    // Não adiciona terminador nulo, pois o campo é fixo
}

// Função para extrair a chave dos 9 primeiros dígitos do CPF
long int extrair_chave(char cpf[TAM_CPF])
{
    long int chave = 0;
    for (int i = 0; i < 9; i++)
    {
        chave = chave * 10 + (cpf[i] - '0');
    }
    return chave;
}

// Gera um CPF único de 11 dígitos (char), garantindo unicidade dos 9 primeiros dígitos
void gerar_cpf_aleatorio(char cpfs_gerados[][TAM_CPF], long int *chaves_usadas, int count, char *cpf)
{
    int unico = 0;
    while (!unico)
    {
        unico = 1;
        for (int i = 0; i < 11; i++)
        {
            cpf[i] = '0' + (rand() % 10);
        }
        long int chave = extrair_chave(cpf);
        // Verifica unicidade dos 9 primeiros dígitos
        for (int i = 0; i < count; i++)
        {
            if (chaves_usadas[i] == chave)
            {
                unico = 0;
                break;
            }
        }
    }
}

int gerar_nota_aleatoria()
{
    return rand() % 101;
}

void gerar_registros()
{
    FILE *arquivo = fopen("registros.dat", "wb");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir arquivo para escrita");
        exit(1);
    }

    Registro reg;
    char cpfs_gerados[TOTAL_REGISTROS][TAM_CPF];
    long int chaves_usadas[TOTAL_REGISTROS];
    for (int i = 0; i < TOTAL_REGISTROS; i++)
    {
        gerar_nome_completo(reg.nome);
        gerar_cpf_aleatorio(cpfs_gerados, chaves_usadas, i, reg.cpf);
        memcpy(cpfs_gerados[i], reg.cpf, TAM_CPF);
        chaves_usadas[i] = extrair_chave(reg.cpf);
        reg.nota = gerar_nota_aleatoria();
        fwrite(&reg, sizeof(Registro), 1, arquivo);
        if ((i + 1) % 1000 == 0)
        {
            printf("Gerados %d registros...\n", i + 1);
        }
    }

    fclose(arquivo);
    printf("Arquivo 'registros.dat' gerado com sucesso!\n");
}

void ler_registros()
{
    FILE *arquivo = fopen("registros.dat", "rb");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir arquivo para leitura");
        exit(1);
    }

    Registro reg;
    int contador = 0;
    printf("\n=== REGISTROS LIDOS DO ARQUIVO ===\n");
    while (fread(&reg, sizeof(Registro), 1, arquivo) == 1)
    {
        printf("Registro %d:\n", ++contador);
        printf("Nome: %.*s\n", TAM_NOME, reg.nome);
        printf("CPF: %.*s\n", TAM_CPF, reg.cpf);
        printf("Nota: %d\n\n", reg.nota);
        if (contador % 20 == 0)
        {
            printf("Pressione Enter para continuar...");
            getchar();
        }
    }
    fclose(arquivo);
    printf("Total de registros lidos: %d\n", contador);
}

int main()
{
    srand(time(NULL));
    int opcao;
    do
    {
        printf("\nMENU:\n");
        printf("1 - Gerar novos registros\n");
        printf("2 - Ler registros existentes\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();
        switch (opcao)
        {
        case 1:
            gerar_registros();
            break;
        case 2:
            ler_registros();
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
    return 0;
}