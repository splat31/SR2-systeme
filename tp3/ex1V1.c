#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>

int NF;

int main(int argc, char* argv[]){
    if (argc != 3){
        fprintf(stderr, "Usage: ./ex1V1 NA NF\n");
        exit(1);
    }

    int NA = atoi(argv[1]);
    NF = atoi(argv[2]);
    int pid;

    for (int i = 0; i < NA; i++){
        switch (pid = fork()){
            case -1:
                perror("Erreur fork\n");
                exit(2);
                break;
            case 0: // Fils
                for (int j = 0; j < NF; j++){
                    printf("Activite rang %d : identifiant = %d\n", i, getpid());
                    sleep(1);
                }
                exit(i);
                break;

            default: // Pere
                break;

        }
    }

    int valRet;
    for (int j = 0; j < NA; j++){
        wait(&valRet);
         printf("Valeur retournee par le fils %d = %d\n", pid, valRet);
    }
    exit(0);
}