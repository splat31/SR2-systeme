#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>

#define NBMAXM 10
typedef struct argu_s {
    int id;
    int nbm;
    int nbl;
    int nbt;
}argu;

sem_t sem[NBMAXM];

void init(int nbt) {
    for (int i = 0; i<nbt ;i++) {
        sem_init(&sem[i],0,0);
    }
    sem_post(&sem[0]);
}

void* routine(void* arg) {
    argu argument = *(argu*)arg;
    srand(pthread_self());
    for (int i = 0; i<argument.nbm;i++) {  
        /*sleep en nano*/
        int sleepRand = rand() % 1500;

        struct timespec ts;
        ts.tv_sec = sleepRand / 1000000;         // secondes
        ts.tv_nsec = (sleepRand % 1000000) * 1000000; // nanosecondes
        nanosleep(&ts, NULL); 
        sem_wait(&sem[argument.id]);
        for (int j = 0; j<argument.nbl; j++) {
            printf("Afficheur %d (%d), j'affiche ligne %d/%d du message %d/%d\n"
                ,argument.id,(int)pthread_self(),j+1,argument.nbl,i+1,argument.nbm);
        }
        sem_post(&sem[(argument.id+1)%argument.nbt]);
    }
    printf("Afficheur %d (%d), je me termine\n",argument.id,(int)pthread_self());
    pthread_exit(NULL);  // on renvoie le résultat
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s nbT NBM NBL\n", argv[0]);
        exit(1);
    }

    int nbt = atoi(argv[1]);
    int nbm = atoi(argv[2]);
    int nbl = atoi(argv[3]);
    pthread_t threads[nbt];
    argu params[nbt];
    init(nbt);
    for (int i = 0; i < nbt; i++) {
        params[i].nbm = nbm;
        params[i].nbl = nbl;
        params[i].id = i;
        params[i].nbt = nbt;
        pthread_create(&threads[i], NULL, routine, &params[i]);
    }


    for (int i = 0; i < nbt; i++) {
        pthread_join(threads[i], NULL);
    }
    /*
    printf("destruction de semaphore\n");
    for (int i = 0; i < nbt; i++) {
        if (sem_trywait(&sem[i]) == 0) {
            sem_post(&sem[i]);
        } else {
            sem_post(&sem[i]);
        }
        sem_destroy(&sem[i]);
    }*/
    printf("Fin du thread principal\n");
    return 0;
}
