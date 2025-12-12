#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int solde;
int nbOp; 

// On remarque que si on ne mets pas de usleep dans les fonctions auxillaires
// alors les collisions d'acces aux var globales (solde) dans les thread sont tres frequentes

void* debit(void* p){
    (void)p;

    srand(pthread_self()); 

    for (int i = 0; i < nbOp; i++){
        int s = rand() % 3;
        sleep(s);
        int randNb = rand() % 10;

        pthread_mutex_lock(&mutex);
        printf("Debit %d : %d - %d => %d\n", (int)pthread_self(), solde, randNb, solde - randNb);
        solde -= randNb;
        if (solde < 0){
            printf("T'es a decouvert mon gadjo\n");
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void* credit(void* p){
    (void)p;

    srand(pthread_self()); 

    for (int i = 0; i < nbOp; i++){
        int s = rand() % 3;
        sleep(s);
        int randNb = rand() % 10;
        pthread_mutex_lock(&mutex);
        printf("Credit %d : %d + %d => %d\n", (int)pthread_self(), solde, randNb, solde+randNb);

        solde += randNb;
        pthread_mutex_unlock(&mutex);
        }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    if (argc != 4){
        fprintf(stderr, "Usage: ./ex2 nbThread soldeInit nbOp\n");
        exit(1);
    }

    solde = atoi(argv[2]);
    int nbThread = atoi(argv[1]);
    nbOp = atoi(argv[3]);
    pthread_t ptid[nbThread];
    //pthread_mutex_init(&mutex, NULL);
    // Creation thread
    for (int i = 0; i < nbThread; i++){
        // paire -> debit
        // impaire -> credit
        if (i % 2 == 0){
            pthread_create(&ptid[i], NULL, debit, NULL);
        } else{
            pthread_create(&ptid[i], NULL, credit, NULL);
        }
    }

    for (int i = 0; i < nbThread; i++){
        pthread_join(ptid[i], NULL);
    }

    if (pthread_mutex_trylock(&mutex) == 0) {
        pthread_mutex_unlock(&mutex);
    } else {
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_destroy(&mutex);

    printf("Solde final: %d\n", solde);
    if (solde < 0){
        printf("LOSER\n");
    }
    exit(0);
}
