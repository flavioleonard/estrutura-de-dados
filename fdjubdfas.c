#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define TOTAL_REGISTROS 10000
#define TAM_NOME 50

typedef struct
{
    char nome[TAM_NOME];
    long long int cpf;
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

void gerar_nome_completo(char *nome_completo)
{
    const char *nome = nomes[rand() % 100];
    const char *sobrenome1 = sobrenomes[rand() % 100];
    const char *sobrenome2 = sobrenomes[rand() % 100];

    // Gera o nome completo
    int tamanho = snprintf(nome_completo, TAM_NOME, "%s %s %s", nome, sobrenome1, sobrenome2);
    
    // Preenche o restante com espaços se necessário
    if (tamanho < TAM_NOME) {
        memset(nome_completo + tamanho, ' ', TAM_NOME - tamanho);
    }
    
    // Garante que o último caractere seja espaço (não nulo)
    nome_completo[TAM_NOME - 1] = ' ';
}

void gerar_cpf_aleatorio(long long int *cpfs_usados, int count, long long int *cpf_gerado)
{
    int unico = 0;
    long long int novo_cpf, nove_digitos_gerado;

    while (!unico)
    {
        unico = 1;
        nove_digitos_gerado = 0;
        for (int i = 0; i < 9; i++)
        {
            nove_digitos_gerado = nove_digitos_gerado * 10 + (rand() % 10);
        }

        int ultimos_dois = rand() % 100;
        novo_cpf = nove_digitos_gerado * 100 + ultimos_dois;

        for (int i = 0; i < count; i++)
        {
            // Pega os 9 primeiros dígitos do CPF já existente para comparação
            long long int chave_existente = cpfs_usados[i] / 100; 
            if (chave_existente == nove_digitos_gerado)
            {
                unico = 0;
                break;
            }
        }
    }
    *cpf_gerado = novo_cpf;
}

int gerar_nota_aleatoria()
{
    return rand() % 101;
}

void gerar_registros()
{
    FILE *arquivo = fopen("registros.dat", "wb");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo para escrita");
        exit(1);
    }

    Registro reg;
    long long int cpfs_usados[TOTAL_REGISTROS];

    for (int i = 0; i < TOTAL_REGISTROS; i++)
    {
        // Gera o nome completo com exatamente 50 caracteres
        gerar_nome_completo(reg.nome);
        
        // Gera os outros campos
        gerar_cpf_aleatorio(cpfs_usados, i, &reg.cpf);
        cpfs_usados[i] = reg.cpf;
        reg.nota = gerar_nota_aleatoria();
        
        // Escreve no arquivo
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
    if (!arquivo)
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
        printf("Nome: [%.*s]\n", TAM_NOME, reg.nome); 
        printf("Tamanho do nome: %zu\n", sizeof(reg.nome)/sizeof(reg.nome[0]));
        printf("CPF: %lld\n", reg.cpf);
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
            printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}