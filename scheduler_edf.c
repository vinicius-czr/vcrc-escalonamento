#include "scheduler_edf.h"

int escolher_proxima_edf(Task tarefas[], int n, int instante_atual) {
    (void)instante_atual;
    int escolhida = -1;

    for (int i = 0; i < n; i++) {
        int pronta = (tarefas[i].rajada_restante > 0);

        if (!pronta) {
            continue;
        }

        if (escolhida == -1) {
            escolhida = i;
            continue;
        }

        if (tarefas[i].deadline_absoluto < tarefas[escolhida].deadline_absoluto) {
            escolhida = i;
        } else if (tarefas[i].deadline_absoluto == tarefas[escolhida].deadline_absoluto &&
                   tarefas[i].ordem_arquivo < tarefas[escolhida].ordem_arquivo) {
            escolhida = i;
        }
    }

    return escolhida;
}

static void fechar_evento_edf(Evento eventos[], int *num_eventos,
                               int indice_tarefa, int duracao, char motivo) {
    if (duracao <= 0) {
        return;
    }
    eventos[*num_eventos].eh_ociosidade = (indice_tarefa == -1);
    eventos[*num_eventos].indice_tarefa = indice_tarefa;
    eventos[*num_eventos].duracao = duracao;
    eventos[*num_eventos].motivo = (indice_tarefa == -1) ? '-' : motivo;
    (*num_eventos)++;
}

int simular_edf(Task tarefas[], int n, int tempo_total,
                 Evento eventos[], Encerramento encerramentos[]) {
    int num_eventos = 0;
    int executando_atual = -1;
    int duracao_atual = 0;

    for (int i = 0; i < n; i++) {
        encerramentos[i].chegadas = 0;
        encerramentos[i].lost = 0;
        encerramentos[i].killed = 0;
    }

    for (int t = 0; t < tempo_total; t++) {
        for (int i = 0; i < n; i++) {
            if (tarefas[i].proxima_chegada == t) {
                tarefas[i].rajada_restante = tarefas[i].burst;
                tarefas[i].deadline_absoluto = t + tarefas[i].deadline;
                encerramentos[i].chegadas++;
            }
        }

        for (int i = 0; i < n; i++) {
            if (tarefas[i].rajada_restante > 0 && tarefas[i].deadline_absoluto == t) {
                tarefas[i].rajada_restante = 0;
                encerramentos[i].lost++;

                if (executando_atual == i) {
                    fechar_evento_edf(eventos, &num_eventos, i, duracao_atual, 'L');
                    executando_atual = -1;
                    duracao_atual = 0;
                }
            }
        }

        int escolhida = escolher_proxima_edf(tarefas, n, t);

        if (escolhida == executando_atual) {
            duracao_atual++;
        } else {
            if (duracao_atual > 0) {
                fechar_evento_edf(eventos, &num_eventos, executando_atual, duracao_atual, 'H');
            }
            executando_atual = escolhida;
            duracao_atual = 1;
        }

        if (escolhida != -1) {
            tarefas[escolhida].rajada_restante--;

            if (tarefas[escolhida].rajada_restante == 0) {
                fechar_evento_edf(eventos, &num_eventos, escolhida, duracao_atual, 'F');
                executando_atual = -1;
                duracao_atual = 0;
            }
        }

        for (int i = 0; i < n; i++) {
            if (tarefas[i].proxima_chegada == t) {
                tarefas[i].proxima_chegada = t + tarefas[i].period;
            }
        }
    }

    fechar_evento_edf(eventos, &num_eventos, executando_atual, duracao_atual, 'H');

    for (int i = 0; i < n; i++) {
        if (tarefas[i].rajada_restante > 0) {
            encerramentos[i].killed = 1;
        }
    }

    return num_eventos;
}