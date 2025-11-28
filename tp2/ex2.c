#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

#define NBFILSMAX 5
#define TIME 2
pid_t fils_pid[NBFILSMAX] = {0} ;
int tubes[2];
bool alarmrecu[NBFILSMAX];
int NBVM;
int NBV;

void handler(int s) {
    pid_t p = getpid();
    if (s==SIGALRM) {
        for (int i = 0;i<NBFILSMAX;i++) {
            if (fils_pid[i]==p) {
                alarmrecu[i]=true;
            }
        }
        alarm(TIME);
    } else if (s==SIGUSR1) {
        printf("Fils %d : fini\n", p);
        exit(0);
    }
}

void fils(int i) {
    struct sigaction s;
	s.sa_handler= handler;//pas sa handler car j'utiliser info et context
	sigemptyset(&(s.sa_mask));
	sigaddset(&(s.sa_mask), SIGALRM);
	s.sa_flags = SA_RESTART;
    if (sigaction(SIGALRM, &s, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    close(tubes[0]); // fermeture lecture
    char c;
    int count = 0;
    alarm(TIME);
    while ((c = getchar()) != EOF && count < NBVM) {
        if (c == '\n') continue; // ignorer les retours à la ligne
        count++;
        if (alarmrecu[i]) {
            while (count - NBV >= 0) {
                char msg[100];
                //TODO FORMAT MESSAGE
                sprintf(msg,"%d %d j'ai recu %d",getpid(),count,count);
                write(tubes[1], &msg, strlen(msg) + 1);
                count=count-NBV;
            }
            alarmrecu[i]=false;
        }
    }
    close(tubes[1]);
    while(1) {
    }
    //Erreur si on arrive la
    exit(0);
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s NF NBVM NBV\n", argv[0]);
        return EXIT_FAILURE;
    }

    int NF = atoi(argv[1]);   // nombre de fils (capteurs)
    if (NF>NBFILSMAX) {
        fprintf(stderr,"Trop de fils en argument\n");
        exit (1);
    }
    NBVM = atoi(argv[2]); // nb max de véhicules par fils
    NBV = atoi(argv[3]);  // nb de véhicules avant envoi



    if (pipe(tubes) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    

    // Création des fils
    for (int i = 0; i < NF; i++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); exit(EXIT_FAILURE); }
        if (pid == 0) {
            fils(i);
        } else {
            // --- Père ---
            fils_pid[i] = pid;
        }
    }
    close(tubes[1]);
    // Père : lecture des fils
    int finis = 0;
    int tabnf[NF];
    for(int ind=0;ind<NF;ind++) {
        tabnf[ind]=0;
    }
    while (finis<NF) {
        char msg[100],msg2[100];
        int a,b;
        read(tubes[0], &msg, sizeof(msg));
        sscanf(msg, "%d %d %s", &a,&b, msg2);
        printf("Père : message recu de %d: %s\n",a, msg2);
        for (int i =0;i<NF;i++) {
            if (fils_pid[i]==(pid_t)a) {
                tabnf[i]=tabnf[i]+b;
                if (tabnf[i]>=NBVM) {
                    finis++;
                    kill(fils_pid[i],SIGUSR1);
                } 
            }
        }

    }

    // Attendre tous les fils
    for (int i = 0; i < NF; i++) wait(NULL);

    printf("Père : tous les fils terminés\n");
    return 0;
}
