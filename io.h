#ifndef IO_H
#define IO_H

#include "task.h"

#define MAX_TAREFAS 32

int ler_tempo_total(const char *caminho_arquivo);

int ler_tarefas(const char *caminho_arquivo, Task tarefas[]);

#endif
