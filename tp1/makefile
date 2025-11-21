# Détection automatique des fichiers tp1-exN.c
SOURCES := $(wildcard tp1-ex*.c)
EXOS := $(patsubst tp1-ex%.c,ex%,$(SOURCES))

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L
#-D_POSIX_C_SOURCE=200809L pour pas a avoir a le remettre a chaque fois



# Règle par défaut : liste des exos disponibles
all:
	@echo "Exercices disponibles : $(EXOS)"
	@echo "Utilisation : make ex<num> (ex : make ex3)"

# Règle générique : make ex3 → compile tp1-ex3.c en tp1-ex3
ex%: tp1-ex%.c
	$(CC) $(CFLAGS) $< -o tp1-$@

# Nettoyage
clean:
	rm -f tp1-ex*
