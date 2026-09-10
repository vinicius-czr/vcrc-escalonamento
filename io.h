#ifndef IO_H
#define IO_H

#include "task.h"

#define MAX_TAREFAS 32

#define ERRO_ARQUIVO -1
#define ERRO_CAMPO_FALTANDO -2
#define ERRO_VALOR_NAO_POSITIVO -3
#define ERRO_REGRA_CDP -4

int ler_tempo_total(const char *caminho_arquivo);

int ler_tarefas(const char *caminho_arquivo, Task tarefas[]);

#endif
