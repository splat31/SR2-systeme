#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s NF NBVM NBV\n", argv[0]);
        return EXIT_FAILURE;
    }

    int NF = atoi(argv[1]);   // nombre de fils (capteurs)
    int NBVM = atoi(argv[2]); // nb max de véhicules par fils
    int NBV = atoi(argv[3]);  // nb de véhicules avant envoi

    int tubes[NF][2];
    pid_t fils_pid[NF];

    // Création des tubes
    for (int i = 0; i < NF; i++) {
        if (pipe(tubes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Création des fils
    for (int i = 0; i < NF; i++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); exit(EXIT_FAILURE); }
        if (pid == 0) {
            // --- Fils ---
            close(tubes[i][0]); // fermeture lecture
            char c;
            int count = 0;
            int send_count = 0;
            while ((c = getchar()) != EOF && count < NBVM) {
                if (c == '\n') continue; // ignorer les retours à la ligne
                count++;
                if (count % NBV == 0) {
                    char msg = "j'ai detecte %d vehicle envoie tous les",count,nbv;
                    write(tubes[i][1], &msg, 1);
                    send_count++;
                }
            }
            close(tubes[i][1]);
            printf("Fils %d : fini\n", getpid());
            exit(0);
        } else {
            // --- Père ---
            fils_pid[i] = pid;
            close(tubes[i][1]); // fermeture écriture côté père
        }
    }

    // Père : lecture des fils
    int finis = 0;
    while (finis < NF) {
        finis = 0;
        for (int i = 0; i < NF; i++) {
            char msg;
            ssize_t n = read(tubes[i][0], &msg, 1);
            if (n > 0) {
                printf("Père : message du fils %d : %c\n", fils_pid[i], msg);
            } else if (n == 0) {
                finis++;
            }
        }
    }

    // Attendre tous les fils
    for (int i = 0; i < NF; i++) wait(NULL);

    printf("Père : tous les fils terminés\n");
    return 0;
}
