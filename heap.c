#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50

typedef struct {
    char nome[MAX_NOME];
    long long int cpf;
    int nota;
} Registro;

int pai(int i) { return i / 2; }
int esq(int i) { return i * 2; }
int dir(int i) { return i * 2 + 1; }

void subir(Registro **heap, int i) {
    int j = pai(i);
    if (j >= 1 && heap[i]->nota > heap[j]->nota) {
        Registro *tmp = heap[i];
        heap[i] = heap[j];
        heap[j] = tmp;
        subir(heap, j);
    }
}

void descer(Registro **heap, int i, int n) {
    int e = esq(i), d = dir(i), maior = i;
    if (e <= n && heap[e]->nota > heap[maior]->nota)
        maior = e;
    if (d <= n && heap[d]->nota > heap[maior]->nota)
        maior = d;
    if (maior != i) {
        Registro *tmp = heap[i];
        heap[i] = heap[maior];
        heap[maior] = tmp;
        descer(heap, maior, n);
    }
}

int insere(Registro ***heap, Registro *novo, int n) {
    *heap = (Registro **) realloc(*heap, sizeof(Registro *) * (n + 2)); // posição 0 ignorada
    if (*heap == NULL) {
        fprintf(stderr, "Erro ao realocar memória da heap\n");
        exit(1);
    }
    (*heap)[n + 1] = novo;
    subir(*heap, n + 1);
    return n + 1;
}

int carregar_registros(const char *arquivoEntrada, Registro ***heap) {
    FILE *fp = fopen(arquivoEntrada, "rb");
    if (!fp) {
        perror("Erro ao abrir registros.dat");
        exit(1);
    }

    Registro temp;
    int n = 0;

    *heap = (Registro **) malloc(sizeof(Registro *) * 1); // posição 0 ignorada
    if (*heap == NULL) {
        fprintf(stderr, "Erro ao alocar memória inicial da heap\n");
        exit(1);
    }

    while (fread(&temp, sizeof(Registro), 1, fp) == 1) {
        Registro *novo = (Registro *) malloc(sizeof(Registro));
        if (novo == NULL) {
            fprintf(stderr, "Erro ao alocar memória para registro\n");
            exit(1);
        }
        *novo = temp;
        n = insere(heap, novo, n);
    }

    fclose(fp);
    return n;
}

void mostrar_ranking(Registro **heap, int *n, int top_k) {
    printf("\nTop %d maiores notas (ranking):\n", top_k);
    
    Registro **heap_copia = (Registro **) malloc(sizeof(Registro *) * (*n + 1));
    int n_copia = *n;
    for (int i = 1; i <= *n; i++) {
        heap_copia[i] = heap[i];
    }

    for (int i = 0; i < top_k && n_copia >= 1; i++) {
        printf("%2dº | Nome: %-50s | CPF: %011lld | Nota: %d\n",
               i + 1, heap_copia[1]->nome, heap_copia[1]->cpf, heap_copia[1]->nota);

        free(heap_copia[1]);  
        heap_copia[1] = heap_copia[n_copia];
        n_copia--;
        descer(heap_copia, 1, n_copia);
    }

    free(heap_copia);
}

void salvar_heap(const char *arquivoSaida, Registro **heap, int n) {
    FILE *fp = fopen(arquivoSaida, "wb");
    if (!fp) {
        perror("Erro ao criar heap.dat");
        exit(1);
    }

    for (int i = 1; i <= n; i++) {
        fwrite(heap[i], sizeof(Registro), 1, fp);
    }

    fclose(fp);
}

int main(void) {
    Registro **heap = NULL;
    int n = carregar_registros("registros.dat", &heap);

    printf("Registros carregados na heap: %d\n", n);

    salvar_heap("heap.dat", heap, n);
    printf("\nHeap salva em 'heap.dat'\n");
    mostrar_ranking(heap, &n, 10000);


    for (int i = 1; i <= n; i++) {
        free(heap[i]);
    }
    free(heap);

    return 0;
}
