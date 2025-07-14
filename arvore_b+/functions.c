
#include "functions.h"
#include <stdio.h>
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
