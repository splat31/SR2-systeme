# Détection automatique des fichiers ex*.c et ex*-prep.c
SOURCES := $(wildcard ex*.c)  # récupère tous les ex*.c
EXOS := $(patsubst %.c,%,$(SOURCES))  # ex1.c -> ex1, ex1-prep.c -> ex1-prep

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L

# Règle par défaut : liste des exos disponibles
all:
	@echo "Exercices disponibles : $(EXOS)"
	@echo "Utilisation : make ex<num> (ex : make ex3) ou make ex<num>-prep"

# Règle générique : compile n'importe quel ex*.c ou ex*-prep.c
%: %.c
	$(CC) $(CFLAGS) $< -o $@

# Nettoyage
clean:
	rm -f ex*
