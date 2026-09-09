CC = gcc
CFLAGS = -Wall -Wextra -std=c11

FONTES = main.c io.c scheduler_rate.c saida.c scheduler_edf.c
OBJETOS = $(FONTES:.c=.o)

scheduler: $(OBJETOS)
	$(CC) $(CFLAGS) -o scheduler $(OBJETOS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJETOS) scheduler
