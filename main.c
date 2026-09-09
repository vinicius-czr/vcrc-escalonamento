#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "task.h"
#include "io.h"
#include "scheduler_rate.h"
#include "saida.h"
#include "scheduler_edf.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "erro: uso incorreto. Esperado: %s <rate|edf> <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    const char *algoritmo = argv[1];
    const char *arquivo_entrada = argv[2];

    if (strcmp(algoritmo, "rate") != 0 && strcmp(algoritmo, "edf") != 0) {
        fprintf(stderr, "erro: algoritmo '%s' invalido. Use 'rate' ou 'edf'\n", algoritmo);
        return 1;
    }

    FILE *teste_abertura = fopen(arquivo_entrada, "r");
    if (teste_abertura == NULL) {
        fprintf(stderr, "erro: nao foi possivel abrir o arquivo '%s': %s\n",
                arquivo_entrada, strerror(errno));
        return 1;
    }
    fclose(teste_abertura);

    int tempo_total = ler_tempo_total(arquivo_entrada);
    if (tempo_total < 0) {
        fprintf(stderr, "erro: arquivo '%s' malformado (tempo total ausente ou invalido)\n", arquivo_entrada);
        return 1;
    }

    Task tarefas[MAX_TAREFAS];
    int n = ler_tarefas(arquivo_entrada, tarefas);
    if (n < 0) {
        fprintf(stderr, "erro: nao foi possivel ler as tarefas do arquivo '%s'\n", arquivo_entrada);
        return 1;
    }

    Evento eventos[MAX_EVENTOS];
    Encerramento encerramentos[MAX_TAREFAS];

    int num_eventos;
    char caminho_saida[64];
    const char *nome_algoritmo;

    if (strcmp(algoritmo, "rate") == 0) {
        num_eventos = simular_rate(tarefas, n, tempo_total, eventos, encerramentos);
        snprintf(caminho_saida, sizeof(caminho_saida), "rate_vcrc.out");
        nome_algoritmo = "RATE";
    } else {
        num_eventos = simular_edf(tarefas, n, tempo_total, eventos, encerramentos);
        snprintf(caminho_saida, sizeof(caminho_saida), "edf_vcrc.out");
        nome_algoritmo = "EDF";
    }

    if (escrever_saida(caminho_saida, nome_algoritmo, tarefas, n, eventos, num_eventos, encerramentos) != 0) {
        fprintf(stderr, "erro: nao foi possivel escrever o arquivo de saida\n");
        return 1;
    }

    return 0;
}
