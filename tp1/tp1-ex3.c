#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage : %s nb_messages periode\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int nb = atoi(argv[1]);
    int periode = atoi(argv[2]);

    /* 1. On bloque SIGINT */
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    printf("SIGINT est bloqué dans le processus principal.\n");
    printf("On remplace maintenant l’image par ./boucler…\n");

    /* 2. Conversion des paramètres en chaînes pour exec */
    char nb_str[16], per_str[16];
    snprintf(nb_str, sizeof(nb_str), "%d", nb);
    snprintf(per_str, sizeof(per_str), "%d", periode);

    /* 3. Remplacement de l’image par boucler */
    execl("./tp1-ex0", "./tp1-ex0", nb_str, per_str, NULL);

    /* Si on arrive ici, c'est que exec a échoué */
    perror("execl");
    return EXIT_FAILURE;
}

//Remarque: execl ne modifie pas les signaux bloque par contre elle 
//reinitialise tout les handlers