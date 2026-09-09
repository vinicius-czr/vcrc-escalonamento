#include <stdio.h>
#include <string.h>
#include "task.h"
#include "io.h"
#include "scheduler_rate.h"
#include "saida.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "uso: %s <rate|edf> <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    const char *algoritmo = argv[1];
    const char *arquivo_entrada = argv[2];

    if (strcmp(algoritmo, "rate") != 0 && strcmp(algoritmo, "edf") != 0) {
        fprintf(stderr, "erro: algoritmo deve ser 'rate' ou 'edf'\n");
        return 1;
    }

    int tempo_total = ler_tempo_total(arquivo_entrada);
    if (tempo_total < 0) {
        fprintf(stderr, "erro: não foi possível ler o arquivo de entrada\n");
        return 1;
    }

    Task tarefas[MAX_TAREFAS];
    int n = ler_tarefas(arquivo_entrada, tarefas);
    if (n < 0) {
        fprintf(stderr, "erro: não foi possível ler as tarefas do arquivo de entrada\n");
        return 1;
    }

    Evento eventos[MAX_EVENTOS];
    Encerramento encerramentos[MAX_TAREFAS];

    if (strcmp(algoritmo, "rate") == 0) {
        int num_eventos = simular_rate(tarefas, n, tempo_total, eventos, encerramentos);

        char caminho_saida[64];
        snprintf(caminho_saida, sizeof(caminho_saida), "rate_vcrc.out");

        if (escrever_saida(caminho_saida, "RATE", tarefas, n, eventos, num_eventos, encerramentos) != 0) {
            fprintf(stderr, "erro: não foi possível escrever o arquivo de saída\n");
            return 1;
        }
    } else {
        fprintf(stderr, "erro: algoritmo edf ainda não implementado\n");
        return 1;
    }

    return 0;
}
