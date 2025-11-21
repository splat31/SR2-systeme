#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NBFILSMAX 5

int tubes[2];
bool alarmrecu[NBFILSMAX] = { false };

void handler(int s) {
    alarmrecu
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
    int send_count = 0;
    while ((c = getchar()) != EOF && count < NBVM) {
        if (c == '\n') continue; // ignorer les retours à la ligne
        count++;
        if (alarmrecu[i]) {
            while (count - NBV >= 0) {
                char msg = "j'ai recu: "+count;
                write(tubes[i][1], &msg, 1);
                count=count-NBV;
            }
        alarmrecu[i]=false;
        }
    }
    //TODO RECEVOIR SIGNAL PERE
    close(tubes[1]);
    printf("Fils %d : fini\n", getpid());
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
    int NBVM = atoi(argv[2]); // nb max de véhicules par fils
    int NBV = atoi(argv[3]);  // nb de véhicules avant envoi

    pid_t fils_pid[NF];

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
            close(tubes[1]); // fermeture écriture côté père
        }
    }

    // Père : lecture des fils
    int finis = 0;
    int tabnf[NF]= {0};
    while () {
        char msg,msg2;
        int a,b;
        ssize_t n = read(tubes[0], &msg, 1);
        sscanf(msg, "%d %d %s", &a, msg);
        printf("Père : message recu de %d: %c\n",a, msg);
        for (int i =0;i<NF;i++) {
            if (fils_pid[i]==(pid_t)a) {
                tabnf[i]=tabnf[i]+b;
                if (tabnf[i]>=NBVMAX) {
                    //TODO envoyer message fils
                } 
            }
        }

        //TODO FINIR BOUCLE en bas ca sert plus a rien


        finis = 0;
        for (int i = 0; i < NF; i++) {
            char msg;
            ssize_t n = read(tubes[0], &msg, 1);
            if (n > 0) {
                printf("Père : message recu : %c\n", msg);
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
