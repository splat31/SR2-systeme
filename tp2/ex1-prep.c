#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s NE\n", argv[0]);
        return EXIT_FAILURE;
    }

    int NE = atoi(argv[1]);
    int tube[2];

    if (pipe(tube) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // --- Fils ---
        close(tube[1]); // fermeture de l'écriture

        char msg;
        ssize_t n;
        while ((n = read(tube[0], &msg, 1)) > 0) {
            printf("Fils %d a reçu un message : %c\n", getpid(), msg);
        }

        close(tube[0]);
        printf("Fils %d : plus de messages, je me termine\n", getpid());
        exit(0);
    } else {
        // --- Père ---
        close(tube[0]); // fermeture de la lecture

        for (int i = 0; i < NE; i++) {
            char msg = 'A' + i; // message symbolique
            write(tube[1], &msg, 1);
            printf("Père %d a envoyé : %c\n", getpid(), msg);
        }

        close(tube[1]); // fermeture du tube pour signaler fin
        wait(NULL);      // attendre le fils
        printf("Père %d : je me termine en dernier\n", getpid());
    }

    return 0;
}
