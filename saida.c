#include <stdio.h>
#include "saida.h"

int escrever_saida(const char *caminho_saida, const char *nome_algoritmo, Task tarefas[], int n, Evento eventos[], int num_eventos, Encerramento encerramentos[]) {
    FILE *f = fopen(caminho_saida, "w");
    if (f == NULL) {
        return -1;
    }

    fprintf(f, "EXECUTION BY %s\n", nome_algoritmo);
    for (int e = 0; e < num_eventos; e++){
        if (eventos[e].eh_ociosidade) {
            fprintf(f, "idle for %d units\n", eventos[e].duracao);
        } else {
            fprintf(f, "[%s] for %d units - %c\n",
                    tarefas[eventos[e].indice_tarefa].nome,
                    eventos[e].duracao,
                    eventos[e].motivo);
        }
    }

    fprintf(f, "\nLOST DEADLINES\n");
    for (int i = 0; i < n; i++) {
        fprintf(f, "[%s] %d\n", tarefas[i].nome, encerramentos[i].lost);
    }

    fprintf(f, "\nCOMPLETE EXECUTION\n");
    for (int i = 0; i < n; i++) {
        int completas = encerramentos[i].chegadas - encerramentos[i].lost - encerramentos[i].killed;
        fprintf(f, "[%s] %d\n", tarefas[i].nome, completas);
    }

    fprintf(f, "\nKILLED\n");
    for (int i = 0; i < n; i++) {
        fprintf(f, "[%s] %d\n", tarefas[i].nome, encerramentos[i].killed);
    }

    fclose(f);
    return 0;
}