// #include "TARVBP.h"

// Dados* ler_dados(FILE* cpf, FILE* nome, FILE* nota) {
//     Dados* d = malloc(sizeof(Dados));
//     memset(d,0,sizeof(Dados));
//     char linha[500] = {0};

//     fgets(linha,sizeof(linha),cpf);
//     linha[strlen(linha)-1] = '\0';
//     strcpy(d->cpf, linha);

//     memset(linha,0x00,sizeof(linha));
//     fgets(linha,sizeof(linha),nome);
//     linha[strlen(linha)-1] = '\0';
//     strcpy(d->nome,linha);

//     memset(linha,0x00,sizeof(linha));
//     fgets(linha,sizeof(linha),nota);
//     linha[strlen(linha)-1] = '\0';
//     d->nota_final = atoi(linha);

//     return d;
// }

// void inic_arq() {
//     FILE* f = fopen("ArqIdx.bin","wb");
//     fclose(f);
//     f = fopen("ArqDados.bin","wb");
//     fclose(f);
// }

// int main(void) {
//     FILE* cpf = fopen("cpf.txt","r");
//     FILE* nome = fopen("nome.txt","r");
//     FILE* nota = fopen("nota.txt","r");
//     int index = 0;
//     char buscas[10001][13];
//     inic_arq();
//     long long raiz = -1;
//     for (int i = 0; i < 1000; i++){
//         Dados* d = ler_dados(cpf,nome,nota);
//         strcpy(buscas[index++],d->cpf);
//         raiz = TARVBP_insere(raiz,d,2,"ArqIdx.bin","ArqDados.bin");
//         printf("cpf: %s raiz:%lld:  %d\n",d->cpf, raiz, i);
//     }
//     printf("buscas:\n");
//     for (int i = 0; i < index;i++) {
//         printf("%d: achamos: %lld\n",i,busca_cpf("ArqIdx.bin",buscas[i],raiz));
//     }
//     fclose(cpf);
//     fclose(nome);
//     fclose(nota);
// }