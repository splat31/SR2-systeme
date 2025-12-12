#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

void* routine(void* arg) {
    int i = *(int*)arg;

    /* graine unique basée sur l'identifiant du thread */
    unsigned int seed = (unsigned int)pthread_self();

    int* res = malloc(sizeof(int));
    if (!res) pthread_exit(NULL);

    *res = rand_r(&seed) % 100;   // nombre entre 0 et 99

    printf("Thread %d : je termine et renvoie %d\n", i, *res);

    pthread_exit(res);  // on renvoie le résultat
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s nThread\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
    pthread_t threads[n];
    int params[n];

    for (int i = 0; i < n; i++) {
        params[i] = i;
        pthread_create(& threads[i], NULL, routine, &params[i]);
    }

    for (int i = 0; i < n; i++) {
        int* retval;
        pthread_join(threads[i], (void**)&retval);

        printf("Mon fils %d s'est terminé en renvoyant %d\n", i, *retval);

        free(retval);  // on libère la mémoire allouée dans le thread
    }

    return 0;
}
