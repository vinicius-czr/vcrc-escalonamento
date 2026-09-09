#ifndef TASK_H
#define TASK_H

#define MAX_NOME 32

typedef struct {
    char nome[MAX_NOME];
    int period;
    int deadline;
    int burst;

    // Itens mutáveis durante a simulação
    int proxima_chegada;
    int deadline_absoluto;
    int rajada_restante;
    int ordem_arquivo;
} Task;

#endif
