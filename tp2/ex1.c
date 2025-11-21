#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

bool termine=false;
int NE;
int i=0;
int tube[2];
int temps;

void handler (int s) {
    (void)s;
    char msg = 'A' + i; // message symbolique
    write(tube[1], &msg, 1);
    printf("Père %d a envoyé : %c\n", getpid(), msg);
    i++;
    if (i>=NE) {
        termine=true;
    } else {
        alarm(temps);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s sec NE\n", argv[0]);
        return EXIT_FAILURE;
    }
    temps=atoi(argv[1]);
    NE = atoi(argv[2]);

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
        //Fils
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
        struct sigaction s;
	    s.sa_handler= handler;//pas sa handler car j'utiliser info et context
	    sigemptyset(&(s.sa_mask));
	    sigaddset(&(s.sa_mask), SIGALRM);
	    s.sa_flags = SA_RESTART;
        if (sigaction(SIGALRM, &s, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
        //Père
        close(tube[0]); // fermeture de la lecture
        alarm(temps);

        while (!termine) {
            printf("Je travaille\n");
            for (int lol=0; lol<100000000;lol++) {
            }//tkt c juste pour pas afficher 40000 fois
            for (int lol=0; lol<100000000;lol++) {
            }//tkt c juste pour pas afficher 40000 fois
            for (int lol=0; lol<100000000;lol++) {
            }//tkt c juste pour pas afficher 40000 fois
            for (int lol=0; lol<100000000;lol++) {
            }//tkt c juste pour pas afficher 40000 fois
        }


        close(tube[1]); // fermeture du tube pour signaler fin
        wait(NULL);      // attendre le fils
        printf("Père %d : je me termine en dernier\n", getpid());
    }

    return 0;
}
