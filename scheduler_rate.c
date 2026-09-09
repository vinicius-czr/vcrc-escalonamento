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

int simular_rate(Task tarefas[], int n, int tempo_total, Evento eventos[]) {
    int num_eventos = 0;
    int executando_atual = -1;
    int duracao_atual = 0;

    for (int t = 0; t < tempo_total; t++) {
        for (int i = 0; i < n; i++) {
            if (tarefas[i].proxima_chegada == t) {
                tarefas[i].rajada_restante = tarefas[i].burst;
                tarefas[i].deadline_absoluto = t + tarefas[i].deadline;
            }
        }

        int escolhida = escolher_proxima_rate(tarefas, n, t);

        if (escolhida == executando_atual) {
            duracao_atual++;
        } else {
            if (duracao_atual > 0) {
                eventos[num_eventos].eh_ociosidade = (executando_atual == -1);
                eventos[num_eventos].indice_tarefa = executando_atual;
                eventos[num_eventos].duracao = duracao_atual;
                num_eventos++;
            }
            executando_atual = escolhida;
            duracao_atual = 1;
        }

        if (escolhida != -1) {
            tarefas[escolhida].rajada_restante--;
        }
        
        for (int i = 0; i < n; i++) {
            if (tarefas[i].proxima_chegada == t) {
                tarefas[i].proxima_chegada = t + tarefas[i].period;
            }
        }
    }

    if (duracao_atual > 0) {
        eventos[num_eventos].eh_ociosidade = (executando_atual == -1);
        eventos[num_eventos].indice_tarefa = executando_atual;
        eventos[num_eventos].duracao = duracao_atual;
        num_eventos++;
    }

    return num_eventos;
}