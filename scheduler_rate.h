#ifndef SCHEDULER_RATE_H
#define SCHEDULER_RATE_H

#include "task.h"

typedef struct {
    int eh_ociosidade;
    int indice_tarefa;
    int duracao;
} Evento;

int escolher_proxima_rate(Task tarefas[], int n, int instante_atual);

int simular_rate(Task tarefas[], int n, int tempo_total, Evento eventos[]);

#endif
