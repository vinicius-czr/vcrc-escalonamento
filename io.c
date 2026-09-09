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

int ler_tarefas(const char *caminho_arquivo, Task tarefas[]) {
    FILE *f = fopen(caminho_arquivo, "r");
    if (f == NULL) {
        return -1;
    }

    int tempo_total;
    if (fscanf(f, "%d", &tempo_total) != 1) {
        fclose(f);
        return -1;
    }

    int quantidade = 0;
    while (quantidade < MAX_TAREFAS &&
           fscanf(f, "%31s %d %d %d",
                  tarefas[quantidade].nome,
                  &tarefas[quantidade].period,
                  &tarefas[quantidade].deadline,
                  &tarefas[quantidade].burst) == 4) {

        tarefas[quantidade].ordem_arquivo = quantidade;
        tarefas[quantidade].proxima_chegada = 0;
        tarefas[quantidade].deadline_absoluto = tarefas[quantidade].deadline;
        tarefas[quantidade].rajada_restante = tarefas[quantidade].burst;

        quantidade++;
    }

    fclose(f);
    return quantidade;
}
