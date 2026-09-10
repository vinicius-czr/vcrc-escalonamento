# vcrc — Simulador de Escalonamento de Tarefas Críticas de Voo

Implementação 3 — Infraestrutura de Software

Simulador que compara dois escalonadores preemptivos clássicos de tempo real —
**rate-monotonic** e **earliest-deadline-first (EDF)** — aplicados a um conjunto
de tarefas periódicas críticas de um computador de bordo.

## Sistema operacional utilizado

Desenvolvido e testado em **WSL (Windows Subsystem for Linux) — Ubuntu**,
usando o VSCode como editor. Compilador `gcc`, padrão C11.

## Arquivos `.c` / `.h` utilizados

| Arquivo | Responsabilidade |
|---|---|
| `task.h` | Define a struct `Task`, que representa uma tarefa periódica (nome, período, deadline, burst) e seu estado mutável durante a simulação (próxima chegada, deadline absoluto atual, rajada restante, ordem no arquivo para desempate). |
| `io.h` / `io.c` | Leitura e parsing do arquivo de entrada: `ler_tempo_total` lê a primeira linha (tempo total de simulação); `ler_tarefas` lê as linhas seguintes, populando o vetor de `Task` e validando formato/positividade/regra `C ≤ D ≤ P`. |
| `scheduler_rate.h` / `scheduler_rate.c` | Escalonador rate-monotonic: `escolher_proxima_rate` decide, a cada instante, qual tarefa pronta tem maior prioridade (menor período vence, desempate por ordem no arquivo); `simular_rate` roda a simulação instante a instante, com preempção, detecção de deadline perdido (`L`), finalização natural (`F`) e preempção (`H`), e gera os contadores de chegadas/lost/killed por tarefa. |
| `scheduler_edf.h` / `scheduler_edf.c` | Escalonador earliest-deadline-first: mesma lógica de `scheduler_rate`, mas o critério de prioridade em `escolher_proxima_edf` é o menor deadline absoluto (em vez de menor período). |
| `saida.h` / `saida.c` | Formata e escreve o arquivo de saída (`rate_vcrc.out` ou `edf_vcrc.out`) no formato exigido pelo enunciado: seções `EXECUTION BY <ALGORITMO>`, `LOST DEADLINES`, `COMPLETE EXECUTION` e `KILLED`. |
| `main.c` | Ponto de entrada: interpreta os argumentos de linha de comando, valida as 5 classes de erro exigidas (argumentos incorretos, algoritmo inválido, arquivo inexistente/ilegível, arquivo malformado, violação de `C ≤ D ≤ P`), executa o escalonador escolhido e chama a escrita da saída. |

## Como compilar

Na raiz do projeto:

```bash
make
```

Isso gera o executável `scheduler` na raiz. Para limpar os artefatos de compilação:

```bash
make clean
```

## Como executar

```bash
./scheduler rate <arquivo_entrada>
./scheduler edf <arquivo_entrada>
```

O programa grava o resultado em `rate_vcrc.out` ou `edf_vcrc.out`, conforme o algoritmo escolhido. Nada é impresso na saída padrão durante a execução normal — mensagens de erro, quando houver, vão para `stderr`, e nenhum arquivo de saída é criado em caso de erro.

Exemplo com o arquivo de exemplo do enunciado:

```bash
./scheduler rate tests/voo.txt
cat rate_vcrc.out
```

## Formato do arquivo de entrada

[TEMPO TOTAL]
[NOME] [PERÍODO] [DEADLINE] [BURST]


Todas as tarefas chegam pela primeira vez no instante 0. Todos os valores devem ser inteiros positivos, respeitando `C ≤ D ≤ P` (burst ≤ deadline ≤ período).

## Arquivos de teste (pasta `tests/`)

| Arquivo | Propósito |
|---|---|
| `voo.txt` | Exemplo oficial do enunciado (tarefas `ATT` e `NAV`), usado para validar rate e EDF contra a saída esperada. |
| `comparativo.txt` | Cenário criado para a análise comparativa obrigatória: tarefas `LENTA` e `RAPIDA` com período próximo mas deadlines muito diferentes, evidenciando um caso em que o rate-monotonic perde deadlines e o EDF não. |
| `erro_campo_faltando.txt` | Testa a classe de erro "campo faltando" (linha de tarefa com menos de 4 campos). |
| `erro_nao_numerico.txt` | Testa a classe de erro "valor não numérico" em um campo de tarefa. |
| `erro_valor_negativo.txt` | Testa a classe de erro "valor não positivo" (período negativo). |
| `erro_regra_cdp.txt` | Testa a violação da regra `C ≤ D ≤ P` (deadline maior que o período). |

## Como testar

Rodar contra o exemplo oficial e comparar com a saída esperada do enunciado:

```bash
make clean && make
./scheduler rate tests/voo.txt && cat rate_vcrc.out
./scheduler edf tests/voo.txt && cat edf_vcrc.out
```

Rodar o cenário comparativo (rate perde deadline, EDF não):

```bash
./scheduler rate tests/comparativo.txt && cat rate_vcrc.out
./scheduler edf tests/comparativo.txt && cat edf_vcrc.out
```

Rodar os testes de erro (cada um deve terminar com código de saída diferente de zero, mensagem em `stderr`, e nenhum arquivo `.out` criado):

```bash
./scheduler rate                                  # número incorreto de argumentos
./scheduler fifo tests/voo.txt                    # algoritmo inválido
./scheduler rate tests/nao_existe.txt             # arquivo inexistente
./scheduler rate tests/erro_campo_faltando.txt    # campo faltando
./scheduler rate tests/erro_nao_numerico.txt      # valor não numérico
./scheduler rate tests/erro_valor_negativo.txt    # valor não positivo
./scheduler rate tests/erro_regra_cdp.txt         # viola C <= D <= P
```

## Observações

O histórico de decisões técnicas, tentativas, erros encontrados (incluindo um bug real de lógica corrigido durante o desenvolvimento) e a análise comparativa completa entre rate-monotonic e EDF estão detalhados no relatório em PDF entregue junto com este diretório.