#ifndef SAIDA_H
#define SAIDA_H

#include "task.h"
#include "scheduler_rate.h"

int escrever_saida(const char *caminho_arquivo, const char *nome_algoritmo, Task tarefas[], int n, Evento eventos[], int num_eventos, Encerramento encerramentos[]);

#endif
