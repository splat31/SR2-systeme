#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <semaphore.h>


typedef struct param_s{
    int id;
    int nbcopain;
} param;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semRDV;


int nbarrive = 0;


void sleepperso(int self) {
    srand(time(NULL) ^ self);
    int sleepRand = rand() % 5000; // jusqu’à 5 secondes

    struct timespec ts;
    ts.tv_sec  = sleepRand / 1000;
    ts.tv_nsec = (sleepRand % 1000) * 1000000;

    nanosleep(&ts, NULL);
}



void* RDV(void* p){
    param paramrdv = *(param*)p;

    printf("Thread %d (%d) : J'effectue un traitement parrallele avec les autres\n",paramrdv.id,(int)pthread_self());
    sleepperso((int)pthread_self());

    printf("Thread %d (%d) : J'arrive au point de RDV\n",paramrdv.id,(int)pthread_self());
    pthread_mutex_lock(&mutex);
    nbarrive++;
    pthread_mutex_unlock(&mutex);
    if (nbarrive == paramrdv.nbcopain) {
        printf("Thread %d (%d) : Je suis le dernier arrive au point de RDV\n",paramrdv.id,(int)pthread_self());
        for (int i = 0; i<paramrdv.nbcopain;i++) {
            sem_post(&semRDV);
        }
    } else {
        printf("Thread %d (%d) : Je ne suis pas le dernier arrive au point de RDV\n",paramrdv.id,(int)pthread_self());
    }

    sem_wait(&semRDV);

    printf("Thread %d (%d) : Je continue un traitement parrallele avec les autres\n",paramrdv.id,(int)pthread_self());
    sleepperso((int)pthread_self());

    printf("Thread %d (%d) : Je termine mon traitement\n",paramrdv.id,(int)pthread_self());
    sem_post(&semRDV);//pour etre sur

    pthread_exit(NULL);
}

    int main(int argc, char* argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: ./ex4 nbThread\n");
        exit(2);
    }

    int nbthread = atoi(argv[1]);
    
    pthread_t ptid[nbthread];
  

    // Creation producteurs
    param tableauParamthreads[nbthread];

    sem_init(&semRDV,0,0);

    for (int i = 0; i < nbthread; i++) {
        tableauParamthreads[i].id=i;
        tableauParamthreads[i].nbcopain=nbthread;
        pthread_create(
            &ptid[i],
            NULL,
            RDV,
            &tableauParamthreads[i]
        );
    }



    for (int i = 0; i < nbthread; i++){
        pthread_join(ptid[i], NULL);
    }

    sem_destroy(&semRDV);
    printf("Fin de l'exécution du thread principal\n");
}