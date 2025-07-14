#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#endif

#define MAX_NOME 50
#define ARQ_HEAP "heap.dat"
#define ARQ_REGS "registros.dat"

typedef struct
{
    char nome[MAX_NOME];
    long long int cpf;
    int nota;
} Registro;

// Protótipo necessário para evitar erro de declaração implícita
int ler_registro(FILE *fp, int pos, Registro *reg);

// Busca um registro pelo CPF no arquivo heap
int buscar_registro_cpf(const char *arq_heap, int n, long long int cpf_busca, Registro *encontrado, int *pos_encontrada)
{
    FILE *fp = fopen(arq_heap, "rb");
    if (!fp)
    {
        perror("Erro ao abrir heap.dat");
        return 0;
    }
    Registro reg;
    for (int i = 1; i <= n; i++)
    {
        if (ler_registro(fp, i, &reg))
        {
            if (reg.cpf == cpf_busca)
            {
                if (encontrado)
                    *encontrado = reg;
                if (pos_encontrada)
                    *pos_encontrada = i;
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

int pai(int i) { return i / 2; }
int esq(int i) { return i * 2; }
int dir(int i) { return i * 2 + 1; }

// Lê um registro na posição pos (1-based) do arquivo
int ler_registro(FILE *fp, int pos, Registro *reg)
{
    return fseek(fp, (pos - 1) * sizeof(Registro), SEEK_SET) == 0 &&
           fread(reg, sizeof(Registro), 1, fp) == 1;
}

// Escreve um registro na posição pos (1-based) do arquivo
int escrever_registro(FILE *fp, int pos, Registro *reg)
{
    return fseek(fp, (pos - 1) * sizeof(Registro), SEEK_SET) == 0 &&
           fwrite(reg, sizeof(Registro), 1, fp) == 1;
}

// Troca dois registros de posição no arquivo
void trocar(FILE *fp, int i, int j)
{
    Registro ri, rj;
    ler_registro(fp, i, &ri);
    ler_registro(fp, j, &rj);
    escrever_registro(fp, i, &rj);
    escrever_registro(fp, j, &ri);
}

void subir(FILE *fp, int i)
{
    int j = pai(i);
    if (j >= 1)
    {
        Registro ri, rj;
        ler_registro(fp, i, &ri);
        ler_registro(fp, j, &rj);
        if (ri.nota > rj.nota)
        {
            trocar(fp, i, j);
            subir(fp, j);
        }
    }
}

void descer(FILE *fp, int i, int n)
{
    int e = esq(i), d = dir(i), maior = i;
    Registro ri, re, rd;
    ler_registro(fp, i, &ri);
    if (e <= n)
    {
        ler_registro(fp, e, &re);
        if (re.nota > ri.nota)
            maior = e;
    }
    if (d <= n)
    {
        ler_registro(fp, d, &rd);
        Registro rmaior;
        ler_registro(fp, maior, &rmaior);
        if (rd.nota > rmaior.nota)
            maior = d;
    }
    if (maior != i)
    {
        trocar(fp, i, maior);
        descer(fp, maior, n);
    }
}

// Constrói a heap máxima no arquivo (heapify)
void constroi_heap_maximo(FILE *fp, int n)
{
    for (int i = n / 2; i >= 1; i--)
        descer(fp, i, n);
}

// Insere um registro na heap em disco
int inserir_registro_heap(const char *arq_heap, int n, Registro *novo)
{
    FILE *fp = fopen(arq_heap, "rb+");
    if (!fp)
    {
        perror("Erro ao abrir heap.dat");
        exit(1);
    }
    // Adiciona no final
    fseek(fp, 0, SEEK_END);
    fwrite(novo, sizeof(Registro), 1, fp);
    fclose(fp);
    // Subida
    fp = fopen(arq_heap, "rb+");
    if (!fp)
    {
        perror("Erro ao abrir heap.dat");
        exit(1);
    }
    subir(fp, n + 1);
    fclose(fp);
    return n + 1;
}

// Exclui a raiz da heap em disco
int excluir_raiz_heap(const char *arq_heap, int n)
{
    if (n < 1)
        return 0;
    FILE *fp = fopen(arq_heap, "rb+");
    if (!fp)
    {
        perror("Erro ao abrir heap.dat");
        exit(1);
    }
    Registro ultimo;
    ler_registro(fp, n, &ultimo);
    escrever_registro(fp, 1, &ultimo);
    fclose(fp);
    // Trunca o arquivo para remover o ultimo
#ifdef _WIN32
    FILE *fp_trunc = fopen(arq_heap, "rb+");
    if (fp_trunc)
    {
        _chsize_s(_fileno(fp_trunc), (n - 1) * sizeof(Registro));
        fclose(fp_trunc);
    }
#else
    truncate(arq_heap, (n - 1) * sizeof(Registro));
#endif
    // Descida
    fp = fopen(arq_heap, "rb+");
    if (!fp)
    {
        perror("Erro ao abrir heap.dat");
        exit(1);
    }
    descer(fp, 1, n - 1);
    fclose(fp);
    return n - 1;
}

// Copia registros.dat para heap.dat (1-based)
int inicializa_heap_arquivo(const char *arq_regs, const char *arq_heap)
{
    FILE *fin = fopen(arq_regs, "rb");
    FILE *fout = fopen(arq_heap, "wb+");
    if (!fin || !fout)
    {
        perror("Erro ao abrir arquivos");
        exit(1);
    }
    Registro reg;
    int n = 0;
    while (fread(&reg, sizeof(Registro), 1, fin) == 1)
    {
        fwrite(&reg, sizeof(Registro), 1, fout);
        n++;
    }
    fclose(fin);
    fclose(fout);
    return n;
}

// Mostra o top k do heap em disco
void mostrar_top_k(const char *arq_heap, int n, int k)
{
    FILE *fp = fopen(arq_heap, "rb");
    if (!fp)
    {
        perror("Erro ao abrir heap.dat");
        exit(1);
    }
    Registro reg;
    printf("\nTop %d maiores notas (em disco):\n", k);
    for (int i = 1; i <= k && i <= n; i++)
    {
        ler_registro(fp, i, &reg);
        // Remove acentos e cedilha dos prints
        printf("%2d | Nome: %-50s | CPF: %011lld | Nota: %d\n", i, reg.nome, reg.cpf, reg.nota);
    }
    fclose(fp);
}

// Função para ler um novo registro do usuário
void ler_novo_registro(Registro *reg)
{
    printf("Digite o nome: ");
    getchar(); // Limpa o \n pendente
    fgets(reg->nome, MAX_NOME, stdin);
    reg->nome[strcspn(reg->nome, "\n")] = '\0'; // Remove \n
    printf("Digite o CPF: ");
    scanf("%lld", &reg->cpf);
    printf("Digite a nota: ");
    scanf("%d", &reg->nota);
}

void menu()
{
    int n = 0;
    int carregado = 0;
    int opcao, k;
    do
    {
        printf("\nMENU:\n");
        printf("1 - Gerar heap maxima em disco\n");
        printf("2 - Mostrar ranking (top k notas)\n");
        printf("3 - Inserir novo registro na heap\n");
        printf("4 - Excluir raiz da heap (maior nota)\n");
        printf("5 - Buscar registro por CPF\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1)
        {
            while (getchar() != '\n')
                ; // limpa buffer
            opcao = -1;
        }
        switch (opcao)
        {
        case 1:
            n = inicializa_heap_arquivo(ARQ_REGS, ARQ_HEAP);
            printf("Registros copiados para heap.dat: %d\n", n);
            {
                FILE *fp = fopen(ARQ_HEAP, "rb+");
                if (!fp)
                {
                    perror("Erro ao abrir heap.dat");
                    exit(1);
                }
                constroi_heap_maximo(fp, n);
                fclose(fp);
            }
            printf("Heap maxima construida em disco!\n");
            carregado = 1;
            break;
        case 2:
            if (!carregado)
            {
                printf("Primeiro gere a heap maxima em disco!\n");
                break;
            }
            printf("Digite o valor de k para o ranking: ");
            if (scanf("%d", &k) != 1 || k <= 0)
            {
                printf("Valor de k invalido!\n");
                while (getchar() != '\n')
                    ;
                break;
            }
            mostrar_top_k(ARQ_HEAP, n, k);
            break;
        case 3:
            if (!carregado)
            {
                printf("Primeiro gere a heap maxima em disco!\n");
                break;
            }
            {
                Registro novo;
                ler_novo_registro(&novo);
                n = inserir_registro_heap(ARQ_HEAP, n, &novo);
                printf("Registro inserido com sucesso! Agora a heap tem %d elementos.\n", n);
            }
            break;
        case 4:
            if (!carregado)
            {
                printf("Primeiro gere a heap maxima em disco!\n");
                break;
            }
            if (n < 1)
            {
                printf("Heap vazia!\n");
                break;
            }
            n = excluir_raiz_heap(ARQ_HEAP, n);
            printf("Raiz excluida! Agora a heap tem %d elementos.\n", n);
            break;
        case 5:
            if (!carregado)
            {
                printf("Primeiro gere a heap maxima em disco!\n");
                break;
            }
            {
                long long int cpf_busca;
                printf("Digite o CPF a buscar: ");
                if (scanf("%lld", &cpf_busca) != 1)
                {
                    printf("CPF invalido!\n");
                    while (getchar() != '\n')
                        ;
                    break;
                }
                Registro encontrado;
                int pos;
                if (buscar_registro_cpf(ARQ_HEAP, n, cpf_busca, &encontrado, &pos))
                {
                    printf("Registro encontrado na posicao %d:\n", pos);
                    printf("Nome: %-50s | CPF: %011lld | Nota: %d\n", encontrado.nome, encontrado.cpf, encontrado.nota);
                }
                else
                {
                    printf("Registro com CPF %lld nao encontrado na heap.\n", cpf_busca);
                }
            }
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

int main()
{
    menu();
    return 0;
}
