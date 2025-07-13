#include <stdio.h>
#include <string.h>

const char* nomes[100] = {
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
    "Renan", "Gilberto", "Valter", "Wellington"
};

const char* sobrenomes[100] = {
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
    " Torres", " Telles", " Paranhos", " Valadão"
};

void gerador(const char **n, const char **s, const char *arquivo){
    char arq[50];
    char result[50];
    strcpy(arq, arquivo);
    strcat(arq, ".bin");
    FILE *f = fopen(arq, "wb");
    for(int i = 0; i < 100; i++){
         strcpy(result, n[i]);
         for(int j = 0; j < 100; j++){
             snprintf(result, sizeof(result),"%s%s", n[i], s[j]);
             fwrite(result, sizeof(char), strlen(result) + 1, f);
         }
    }
    fclose(f);
}

