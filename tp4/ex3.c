#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <semaphore.h>


#define TAB_MAX_SIZE 30

typedef struct param_s{
 int rang;
 int nbTrucAFaire;
} param;

pthread_mutex_t mutex;
sem_t semnbCasePleine,semnbCaseVide;

char tab[TAB_MAX_SIZE][30];
int tailleTab;
int prochaineCaseVide = 0;
int prochaineCasePleine = 0;

void init(int taille) {
    pthread_mutex_init(&mutex, NULL);
    sem_init(&semnbCasePleine,0,0);
    sem_init(&semnbCaseVide,0,taille);
}
void sleepperso(int self) {
    srand(time(NULL) ^ self);
    int sleepRand = rand() % 1000; // jusqu’à 3 secondes

    struct timespec ts;
    ts.tv_sec  = sleepRand / 1000;
    ts.tv_nsec = (sleepRand % 1000) * 1000000;

    nanosleep(&ts, NULL);
}

void* production(void* p){

    char message[20];
    param lesTrucs = *(param*)p;

    for (int i = 0; i < lesTrucs.nbTrucAFaire; i++){
        sleepperso((int)pthread_self());

        sem_wait(&semnbCaseVide);
        pthread_mutex_lock(&mutex);
        
        sprintf(message, "Bonjour %d de %d\n", i, lesTrucs.rang);
        strcpy(tab[prochaineCaseVide], message);

        prochaineCaseVide = (prochaineCaseVide + 1) % tailleTab;

        printf("Prod %d (%d) : Message depose = %s\n", lesTrucs.rang, (int)pthread_self(), message);
        pthread_mutex_unlock(&mutex);
        sem_post(&semnbCasePleine);   
    }
    pthread_exit(NULL);
}

void* conssomation(void* p){

    char message[20],message2[20];
    param lesTrucs = *(param*)p;

    for (int i = 0; i < lesTrucs.nbTrucAFaire; i++){

        sleepperso((int)pthread_self());

        sem_wait(&semnbCasePleine);
        pthread_mutex_lock(&mutex);

        strcpy(message, tab[prochaineCasePleine]);
        sprintf(message2, "Case Vide\n");
        strcpy(tab[prochaineCasePleine], message2);
            
        prochaineCasePleine = (prochaineCasePleine + 1) % tailleTab;

        printf("\tConsso %d (%d) : Message retire = %s\n", lesTrucs.rang, (int)pthread_self(), message);
           
        pthread_mutex_unlock(&mutex);
        sem_post(&semnbCaseVide);
    }

    pthread_exit(NULL);
}

    int main(int argc, char* argv[]){
    if (argc != 6){
        fprintf(stderr, "Usage: ./ex3 nbProd nbCons nbDepot nbConssomation tabSize\n");
        exit(3);
    }

    int nbProducteur = atoi(argv[1]);
    int nbConssomateurs = atoi(argv[2]);
    int nbDepot = atoi(argv[3]);
    int nbConssomation = atoi(argv[4]);
    tailleTab = atoi(argv[5]);
    if (nbDepot < nbConssomation) {
        printf("cas nbconssommation > nb depot pas encore gere : solution envisage -> utilisation timewait\n");
        printf("Le code n'as pas de terminaison normale dans ce cas\n");
        sleep(1);
    }

    pthread_t ptid[nbProducteur + nbConssomateurs];
    int indexPtid = 0;

    // Creation producteurs
    param tableauParamProducteurs[nbProducteur];
    init(tailleTab);
    for (int i = 0; i < nbProducteur; i++) {
        tableauParamProducteurs[i].nbTrucAFaire = nbDepot;
        tableauParamProducteurs[i].rang = i;

        pthread_create(
            &ptid[indexPtid],
            NULL,
            production,
            &tableauParamProducteurs[i]
        );
        indexPtid++;
    }


    // Creation conssomateurs
    param tableauParamConsommateurs[nbConssomateurs];

    for (int i = 0; i < nbConssomateurs; i++) {
        tableauParamConsommateurs[i].nbTrucAFaire = nbConssomation;
        tableauParamConsommateurs[i].rang = i;

        pthread_create(
            &ptid[indexPtid],
            NULL,
            conssomation,
            &tableauParamConsommateurs[i]
        );
        indexPtid++;
    }


    for (int i = 0; i < nbProducteur + nbConssomateurs; i++){
        pthread_join(ptid[i], NULL);
    }

    pthread_mutex_trylock(&mutex);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);

    sem_trywait(&semnbCaseVide);
    sem_trywait(&semnbCasePleine);

    sem_post(&semnbCaseVide);
    sem_post(&semnbCasePleine);

    sem_destroy(&semnbCaseVide);
    sem_destroy(&semnbCasePleine);
    printf("cest moi qui gagne\n");
}
