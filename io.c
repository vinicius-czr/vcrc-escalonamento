#include <stdio.h>
#include "io.h"

int ler_tempo_total(const char *caminho_arquivo) {
    FILE *f = fopen(caminho_arquivo, "r");
    if (f == NULL) {
        return ERRO_ARQUIVO;
    }

    int tempo_total;
    int lidos = fscanf(f, "%d", &tempo_total);
    fclose(f);

    if (lidos != 1) {
        return ERRO_CAMPO_FALTANDO;
    }
    if (tempo_total <= 0) {
        return ERRO_VALOR_NAO_POSITIVO;
    }

    return tempo_total;
}

int ler_tarefas(const char *caminho_arquivo, Task tarefas[]) {
    FILE *f = fopen(caminho_arquivo, "r");
    if (f == NULL) {
        return ERRO_ARQUIVO;
    }

    int tempo_total;
    if (fscanf(f, "%d", &tempo_total) != 1) {
        fclose(f);
        return ERRO_CAMPO_FALTANDO;
    }

    int quantidade = 0;
    char linha_nome[MAX_NOME];
    int linha_period, linha_deadline, linha_burst;
    int campos_lidos;

    while ((campos_lidos = fscanf(f, "%31s %d %d %d",
                                   linha_nome, &linha_period,
                                   &linha_deadline, &linha_burst)) != EOF) {

        if (campos_lidos != 4) {
            fclose(f);
            return ERRO_CAMPO_FALTANDO;
        }

        if (linha_period <= 0 || linha_deadline <= 0 || linha_burst <= 0) {
            fclose(f);
            return ERRO_VALOR_NAO_POSITIVO;
        }

        if (linha_burst > linha_deadline || linha_deadline > linha_period) {
            fclose(f);
            return ERRO_REGRA_CDP;
        }

        if (quantidade >= MAX_TAREFAS) {
            fclose(f);
            return ERRO_CAMPO_FALTANDO;
        }

        snprintf(tarefas[quantidade].nome, MAX_NOME, "%s", linha_nome);
        tarefas[quantidade].period = linha_period;
        tarefas[quantidade].deadline = linha_deadline;
        tarefas[quantidade].burst = linha_burst;
        tarefas[quantidade].ordem_arquivo = quantidade;
        tarefas[quantidade].proxima_chegada = 0;
        tarefas[quantidade].deadline_absoluto = linha_deadline;
        tarefas[quantidade].rajada_restante = linha_burst;

        quantidade++;
    }

    fclose(f);
    return quantidade;
}
