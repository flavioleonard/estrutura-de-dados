#include "TARVBP.h"
int nos_atual = 0;

TARVBP TARVBP_cria(char* idx) {
    TARVBP no;
    no.folha = 1;
    no.num_chaves = 0;
    int i = 0;
    for(; i < TAM * 2 - 1; i++){
    	strcpy(no.chaves[i],"-1");
        no.filhos[i] = -1;
   		no.reg[i] = -1;
    }
    no.end = nos_atual*sizeof(TARVBP);
    no.filhos[i] = -1;
    no.proximo_no = -1; // -1 indica que não há próximo
    return no;
}


void ler_no(char* arq, long long reg, TARVBP *no){
    FILE* arq_i = fopen(arq, "rb");
    fseek(arq_i, reg, SEEK_SET);
    fread(no, sizeof(TARVBP), 1, arq_i);
    fclose(arq_i);
}

long long busca_cpf(char* arq_idx, char* cpf, long long reg) {
    if (!arq_idx) return -1;

    long long reg_atual = reg;
    TARVBP no_atual;

    while (reg_atual != -1) {
        ler_no(arq_idx, reg_atual, &no_atual);

        if (no_atual.folha) {
            for (int i = 0; i < no_atual.num_chaves; i++) {
                if (!strcmp(no_atual.chaves[i],cpf)) {
                    return no_atual.reg[i];
                }
            }
            return -1;
        }
        int i = 0;
        while (i < no_atual.num_chaves && strcmp(no_atual.chaves[i],cpf) <= 0) i++;
        reg_atual = no_atual.filhos[i];
    }
    return -1;
}

void escrever_no(char *arq, long long reg, TARVBP* no){
    FILE* arq_i = fopen(arq, "rb+");
    fseek(arq_i, reg, SEEK_SET);
    fwrite(no, sizeof(TARVBP), 1, arq_i);
    fclose(arq_i);
}

TARVBP divisao(TARVBP a, TARVBP p, int t, int i,char* idx){
    nos_atual++;
    TARVBP z = TARVBP_cria(idx);
    z.folha = a.folha;
    if(!a.folha){
        z.num_chaves = t-1;
        for(int j = 0;j<t-1;j++){
            strcpy(z.chaves[j],a.chaves[j+t]);
        }
        for(int k = 0;k<t;k++){
            z.filhos[k] = a.filhos[t+k];
            a.filhos[t+k] = -1;
        }
    }
    else{
        z.num_chaves = t;
        for(int j = 0;j<t;j++){
            z.reg[j] = a.reg[j+t-1];
            strcpy(z.chaves[j], a.chaves[j+t-1]);
        }
        z.proximo_no = a.proximo_no;
        a.proximo_no = z.end;
    }
    for(int j = p.num_chaves;j>i-1;j--){
        p.filhos[j+1] = p.filhos[j];
    }
    p.filhos[i] = z.end;
    for(int k = p.num_chaves;k>i-1;k--){
        strcpy(p.chaves[k],p.chaves[k-1]);
    }
    strcpy(p.chaves[i-1],a.chaves[t-1]);
    a.num_chaves=t-1;
    p.num_chaves++;
    for(int jk = t-1;jk< (2*t) -1;jk++){
        a.reg[jk] = -1;
        strcpy(a.chaves[jk],"-1");
    }
    escrever_no(idx,a.end,&a);
    escrever_no(idx,z.end,&z);
    escrever_no(idx,p.end,&p);
    return p;
}


TARVBP insere_n_completo(TARVBP a, Dados* d, int t, char* idx, char* dados){
    int i;
    if(a.folha){ //finalmente insere de fato
        i = a.num_chaves-1;
        while((i>-1)&& (strcmp(d->cpf,a.chaves[i])<0)){
            strcpy(a.chaves[i+1],a.chaves[i]);
            a.reg[i+1] = a.reg[i];
            i--;
        }
        strcpy(a.chaves[i+1],d->cpf);
        a.num_chaves++;
        FILE* arq_dados = fopen(dados, "rb+");
        fseek(arq_dados, 0, SEEK_END);
        a.reg[i+1] = ftell(arq_dados);
        fwrite(d, sizeof(Dados),1,arq_dados);
        escrever_no(idx,a.end,&a);
        fclose(arq_dados);
        return a;
    }
    i = a.num_chaves-1;
    while((i>-1) && (strcmp(d->cpf ,a.chaves[i]) < 0) ){
        i--;
    }
    TARVBP f;
    ler_no(idx, a.filhos[i+1],&f);
    i++;
    int div = 0;
    if(f.num_chaves == ((2*t)-1)){ // no cheio
        a = divisao(f,a,t,i+1,idx);
        div = 1;
        if(strcmp(a.chaves[i], d->cpf) < 0)i++;
    }
    if(div){    //divisao foi feita
        ler_no(idx, a.filhos[i],&f);
    }
    f = insere_n_completo(f, d, t,idx,dados);
    escrever_no(idx,f.end,&f);
    return a;
}


long long TARVBP_insere(long reg, Dados* d, int t, char* idx, char* dados){
    TARVBP a;
    if (reg != -1)ler_no(idx,reg, &a);
    else a = TARVBP_cria(idx);
    if(busca_cpf(idx, d->cpf,a.end) != -1)return a.end;
    if(a.num_chaves != (2*t)-1){
        a = insere_n_completo(a, d, t, idx, dados);
        return a.end;
    }
    nos_atual++;
    TARVBP p = TARVBP_cria(idx);
    p.filhos[0] = a.end;
    p.folha = 0;
    p = divisao(a,p,t,1,idx);
    p = insere_n_completo(p, d, t,idx, dados);
    return p.end;
}


