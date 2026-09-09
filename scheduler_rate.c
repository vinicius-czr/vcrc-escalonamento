#include "scheduler_rate.h"

int escolher_proxima_rate(Task tarefas[], int n, int instante_atual) {
    int escolhida = -1;

    for (int i = 0; i < n; i++){
        int pronta = (tarefas[i].proxima_chegada <= instante_atual) && (tarefas[i].rajada_restante > 0);

        if (!pronta) {
            continue;
        }

        if (escolhida == -1) {
            escolhida = i;
            continue;
        }

        if (tarefas[i].period < tarefas[escolhida].period) {
            escolhida = i;
        } else if (tarefas[i].period == tarefas[escolhida].period && tarefas[i].ordem_arquivo < tarefas[escolhida].ordem_arquivo){
            escolhida = i;
        }
    }

    return escolhida;
}