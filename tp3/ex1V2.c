#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <pthread.h>

#define NA_MAX 10

int NF;
pthread_t ptid[NA_MAX];
int valRet[NA_MAX];


void* starting_routine(void* p){ // On passe NF en param
    int* nf = (int*)p;

    int rang;
    if (pthread_self() == ptid[0]){
        rang = 0;
    } else {
        rang = 1;
    }

    for (int i = 0; i < *nf; i++){
        printf("Activite rang %d : identifiant = %d\n", rang, (int)pthread_self()); // Trouver un moyen d'avoir acces au rand (struct?)
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    if (argc != 3){
        fprintf(stderr, "Usage: ./ex1V1 NA NF\n");
        exit(1);
    }

    int NA = atoi(argv[1]);
    NF = atoi(argv[2]);
    int param[NA];
    for (int i = 0; i < NA; i++){
        param[i] = NF;
        pthread_create(&ptid[i], NULL, starting_routine, (void*)&param[i]);
    }

    for (int i = 0; i < NA; i++){
        pthread_join(ptid[i], (void**)&valRet[i]); // A verifier
        printf("Valeur retournee par le fils %d = %d\n", (int)ptid[i], i);
    }

    return 0;
}