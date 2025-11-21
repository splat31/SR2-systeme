#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define NBFILS 2
int nb_car;
int nb_car_max;
char c;
void fils(int n, int m) {
    int nblu=0;
    pid_t pere = getppid();
    while ((c=getchar()) != EOF&&nblu<m) {
        if (c=='\n') {
            continue;
        }
        nblu++;
        if (nblu%n==0) {
            kill(pere,SIGUSR1);
        }
    }
    printf("[%d] Fils: J'ai lu mon maximum je me termine\n",getpid());
    return;
}


void mafonction (int sig, siginfo_t *info, void *context) {
    //Evite warning
    (void)sig;
    (void)context; 

    printf("[%d] Pere: Mon fils %d a recu %d caractere\n",getpid(),info->si_pid,nb_car);
	
}

int main (int argc, char * argv[]) {
    if (argc!=3) {
        fprintf(stderr, "Usage : %s nb_car_per_send nb_car_max\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    nb_car=atoi(argv[1]);
    nb_car_max=atoi(argv[2]);

    for (int i =0; i<NBFILS;i++) {
        pid_t pid = fork();
        if (pid<0) {
            perror("fork");
            exit(1);
        } if (pid==0) {
            fils(nb_car,nb_car_max);
            return 0;
        }
    }

    struct sigaction s;
	s.sa_sigaction= mafonction;//pas sa handler car j'utiliser info et context
	sigemptyset(&(s.sa_mask));
	sigaddset(&(s.sa_mask), SIGUSR1);
	s.sa_flags = SA_SIGINFO | SA_RESTART;
    //si erreur doit kill fils j'ai pas fait
	if (sigaction(SIGUSR1, &s, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    struct sigaction s2;
	s2.sa_handler= mafonction;//pas sa handler car j'utiliser info et context
	sigemptyset(&(s2.sa_mask));
	sigaddset(&(s2.sa_mask), SIGALRM);
	s2.sa_flags = SA_SIGINFO | SA_RESTART;
    if (sigaction(SIGALRM, &s, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sigset_t oldmask;
    sigset_t blockmask;
    sigemptyset(&blockmask);
    sigaddset(&blockmask, SIGUSR1);
    while ()


    pid_t pidpere = getpid();
    printf("[%d] Pere: Masque mis en place\n",pidpere);

    for (int i =0; i<NBFILS;i++) {
        wait(NULL);
    }
    printf("[%d] Pere: Je me termine en dernier\n",pidpere);
    return 0;
}