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
int nbCasePleine = 0;

void init(int taille) {
    pthread_mutex_init(&mutex, NULL);
    sem_init(&semnbCasePleine,0,0);
    sem_init(&semnbCaseVide,0,taille);
}

void* production(void* p){
 srand(pthread_self());
 char message[20];
 param lesTrucs = *(param*)p;

 for (int i = 0; i < lesTrucs.nbTrucAFaire; i++){
 int sleepRand = rand() % 1500;

 struct timespec ts;
 ts.tv_sec = sleepRand / 1000000;         // secondes
 ts.tv_nsec = (sleepRand % 1000000) * 10000; // nanosecondes
 nanosleep(&ts, NULL);

 if (nbCasePleine < tailleTab){
 sprintf(message, "Bonjour %d de %d\n", i, lesTrucs.rang);
 strcpy(tab[prochaineCaseVide], message);
 

 prochaineCaseVide = (prochaineCaseVide + 1) % tailleTab;
 nbCasePleine++;

 printf("Prod %d (%d) : Message depose = %s\n", lesTrucs.rang, (int)pthread_self(), message);
 } else {
 printf("Prod : nbCasePleine >= tailleTab");
 }
 }
 pthread_exit(NULL);
}

void* conssomation(void* p){
 srand(pthread_self());
 char message[20];
 param lesTrucs = *(param*)p;

 for (int i = 0; i < lesTrucs.nbTrucAFaire; i++){
 int sleepRand = rand() % 1500;

 struct timespec ts;
 ts.tv_sec = sleepRand / 1000000;         // secondes
 ts.tv_nsec = (sleepRand % 1000000) * 1000000; // nanosecondes
 nanosleep(&ts, NULL); 
 if (nbCasePleine > 0){
 strcpy(message, tab[prochaineCasePleine]);
 prochaineCasePleine = (prochaineCasePleine + 1) % tailleTab;
 nbCasePleine--;

 printf("\tConsso %d (%d) : Message retire = %s\n", lesTrucs.rang, (int)pthread_self(), message);
 } else {
 printf("consso : nbCasePleine <= 0\n");
 }
 }
 pthread_exit(NULL);
}

int main(int argc, char* argv[]){
 if (argc != 6){
 fprintf(stderr, "Usage: ./ex3 nbProd nbCons nbDepot nbConssomation tabSize\n");
 exit(43);
 }

 int nbProducteur = atoi(argv[1]);
 int nbConssomateurs = atoi(argv[2]);
 int nbDepot = atoi(argv[3]);
 int nbConssomation = atoi(argv[4]);
 tailleTab = atoi(argv[5]);

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

 printf("cest moi qui gagne\n");
}
