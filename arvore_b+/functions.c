#include "functions.h"

const char *a[100] = {
    "020066", "032436", "036776", "046828", "049797", "052067", "057840", "060720", "065416", "066827",
    "077429", "082580", "097092", "101447", "107177", "129259", "142233", "146670", "166184", "178973",
    "190635", "197783", "213007", "219737", "246510", "249009", "271363", "276942", "283749", "299363",
    "304459", "310311", "331869", "335179", "352701", "358971", "373665", "384855", "385695", "411796",
    "413542", "436399", "449195", "455853", "473919", "475819", "479367", "482484", "495766", "507264",
    "510821", "511857", "526769", "532543", "537458", "567931", "569910", "576879", "577755", "605817",
    "615063", "618445", "630109", "631061", "654569", "658876", "664334", "664414", "668317", "669283",
    "674794", "676215", "678119", "691343", "691605", "692453", "698127", "702671", "719425", "721734",
    "725255", "735597", "754339", "761849", "765241", "781521", "788761", "791059", "808963", "811452",
    "831170", "831928", "868337", "898946", "900214", "946129", "965690", "967114", "977310", "985226"};

const char *b[100] = {
    "00583", "01350", "02383", "04492", "04544", "05141", "08540", "09214", "10795", "12049",
    "14479", "15351", "16358", "16409", "16773", "19868", "22192", "22412", "23933", "24108",
    "24419", "25094", "27214", "28577", "29790", "30065", "32149", "32856", "33687", "33863",
    "33922", "33927", "35199", "37241", "37676", "38255", "38765", "39104", "40211", "41928",
    "44284", "45692", "47290", "47568", "48169", "49095", "49570", "51615", "52466", "54432",
    "54945", "55680", "55997", "56957", "57030", "59781", "59945", "60659", "63442", "64232",
    "64299", "66072", "67263", "67273", "68275", "68635", "69347", "69469", "69686", "69767",
    "69809", "71952", "72397", "76249", "77611", "77923", "78488", "78527", "79798", "80213",
    "81671", "82075", "84890", "86695", "86754", "87038", "87167", "87979", "90720", "91160",
    "91512", "92429", "92466", "92526", "92949", "93430", "94210", "94371", "97294", "98955"};

const char *nomes[100] = {
    "Vitor", "Gabriel", "Caio", "Lucas", "Alex", "Miguel",
    "Ana", "Maria", "Raquel", "Beatriz", "Laura", "Helena",
    "Arthur", "Matheus", "Pedro", "Augusto", "Gustavo", "Felipe",
    "Bruno", "Leonardo", "Rafael", "Thiago", "Daniel", "André",
    "Igor", "Eduardo", "Henrique", "Vinicius", "Diego", "Samuel",
    "Isabela", "Sophia", "Valentina", "Lorena", "Yasmin", "Carolina",
    "Mariana", "Lívia", "Giovanna", "Eloá", "Fernanda", "Larissa",
    "Lunna", "Clara", "Cecília", "Isadora", "Esther", "Olívia",
    "Elias", "Otávio", "Murilo", "Davi", "Fernando", "Carlos",
    "Marcela", "Melissa", "Paula", "Mirela", "Tatiane", "Luiza",
    "Juliana", "Camille", "Renata", "Elisa", "Daniele", "Patrícia",
    "Sabrina", "Aline", "Vanessa", "Priscila", "Érika", "Milena",
    "Simone", "Bianca", "Adriana", "Cristiane", "Grazi", "Natália",
    "Cael", "Noah", "Hiago", "Luiz", "Vicente", "Cristian",
    "Jonathan", "Érico", "Edson", "Israel", "Rian", "Anderson",
    "Mateus", "Emanuel", "Victor", "Raul", "Pablo", "Alan",
    "Renan", "Gilberto", "Valter", "Wellington"};

const char *sobrenomes[100] = {
    " Lemos", " Baeta", " Conceição", " Duarte", " Gavião", " Guimarães",
    " Ramos", " Silva", " Santos", " Oliveira", " Souza", " Pereira",
    " Lima", " Ferreira", " Almeida", " Costa", " Carvalho", " Rocha",
    " Dias", " Martins", " Barbosa", " Moura", " Melo", " Cavalcante",
    " Batista", " Campos", " Monteiro", " Nogueira", " Teixeira", " Farias",
    " Sales", " Cardoso", " Andrade", " Rezende", " Correia", " Pinto",
    " Azevedo", " Tavares", " Peixoto", " Valverde", " Cunha", " Figueiredo",
    " Vieira", " Antunes", " Castro", " Fonseca", " Pacheco", " Araújo",
    " Seabra", " Assis", " Bittencourt", " Mendes", " Soares", " Queiroz",
    " Amorim", " Amado", " Vasconcelos", " Siqueira", " Ribeiro", " Braga",
    " Magalhães", " Cavalcanti", " Chaves", " Barros", " Porto", " Xavier",
    " Gonçalves", " Esteves", " Bernardes", " Macedo", " Neves", " Pedrosa",
    " Prado", " Freitas", " Camargo", " Moraes", " Beltrão", " Brandão",
    " Corrêa", " Coelho", " Diniz", " Siqueira", " Salazar", " Garcia",
    " Moreira", " Borges", " Aguiar", " Barreto", " Lacerda", " Serpa",
    " Machado", " Miranda", " Leal", " Henriques", " Varela", " Saldanha",
    " Torres", " Telles", " Paranhos", " Valadão"};

// Inicializa arquivos de índice e dados
void inic_arq()
{
    FILE *f = fopen("bplus_index.dat", "wb");
    if (f)
        fclose(f);
    f = fopen("bplus_dados.dat", "wb");
    if (f)
        fclose(f);
}

void gerador(const char **n, const char **s, const char *arquivo)
{
    char arq[50];
    char result[50];
    strcpy(arq, arquivo);
    strcat(arq, ".dat");
    FILE *f = fopen(arq, "wb");
    for (int i = 0; i < 100; i++)
    {
        strcpy(result, n[i]);
        for (int j = 0; j < 100; j++)
        {
            snprintf(result, sizeof(result), "%s%s", n[i], s[j]);
            fwrite(result, sizeof(char), strlen(result) + 1, f);
        }
    }
    fclose(f);
}

void gerador_nota(const char *arquivo)
{
    char arq[50];
    int result = 0;
    strcpy(arq, arquivo);
    strcat(arq, ".dat");
    FILE *f = fopen(arq, "wb");
    for (int i = 0; i < 10000; i++)
    {
        result = rand() % 101;
        fwrite(&result, sizeof(int), 1, f);
    }
    fclose(f);
}

void leitura_escrita(const char *arquivo_bin, const char *arquivo_txt)
{
    char arq_b[50], arq_t[50], var[50];
    strcpy(arq_b, arquivo_bin);
    strcat(arq_b, ".dat");
    strcpy(arq_t, arquivo_txt);
    strcat(arq_t, ".txt");
    FILE *f = fopen(arq_b, "rb");
    FILE *a = fopen(arq_t, "w");
    while (fread(var, sizeof(char), 1, f))
    {
        int i = 0;
        while (var[i] != '\0')
        {
            i++;
            fread(&var[i], sizeof(char), 1, f);
        }
        fprintf(a, "%s\n", var);
    }
    fclose(a);
    fclose(f);
}

void leitura_escrita_nota(const char *arquivo_bin, const char *arquivo_txt)
{
    char arq_b[50], arq_t[50];
    int result = 0;
    strcpy(arq_b, arquivo_bin);
    strcat(arq_b, ".dat");
    strcpy(arq_t, arquivo_txt);
    strcat(arq_t, ".txt");
    FILE *f = fopen(arq_b, "rb");
    FILE *a = fopen(arq_t, "w");
    while (fread(&result, sizeof(int), 1, f))
    {
        fprintf(a, "%d\n", result);
    }
    fclose(a);
    fclose(f);
}
