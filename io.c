#include <stdio.h>
#include "io.h"

int ler_tempo_total(const char *caminho_arquivo) {
    FILE *f = fopen(caminho_arquivo, "r");
    if (f == NULL) {
        return -1;
    }

    int tempo_total;
    int lidos = fscanf(f, "%d", &tempo_total);
    fclose(f);

    if (lidos != 1) {
        return -1;
    }

    return tempo_total;
}
