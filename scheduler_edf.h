#ifndef SCHEDULER_EDF_H
#define SCHEDULER_EDF_H

#include "task.h"
#include "scheduler_rate.h"

int escolher_proxima_edf(Task tarefas[], int n, int instante_atual);

int simular_edf(Task tarefas[], int n, int tempo_total, Evento eventos[], Encerramento encerramentos[]);

#endif
