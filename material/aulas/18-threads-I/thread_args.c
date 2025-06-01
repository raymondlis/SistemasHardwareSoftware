// threads_args.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *tarefa_print_i(void *arg) {
    int *i = (int *)arg;               // converte o void* de volta para int*
    printf("Thread %lu recebeu i = %d\n",
           (unsigned long)pthread_self(), *i);
    return NULL;
}

int main(void) {
    // 1) aloca o vetor de int e o vetor de pthread_t
    int  *vi   = malloc(4 * sizeof(int));
    pthread_t *tids = malloc(4 * sizeof(pthread_t));
    if (!vi || !tids) {
        perror("malloc");
        return 1;
    }

    // 2) cria 4 threads, guarda o índice em vi[j] e passa &vi[j]
    for (int j = 0; j < 4; j++) {
        vi[j] = j;
        int err = pthread_create(&tids[j], NULL, tarefa_print_i, &vi[j]);
        if (err) {
            fprintf(stderr, "Erro pthread_create: %d\n", err);
            return 1;
        }
    }

    // 3) espera cada thread terminar
    for (int j = 0; j < 4; j++) {
        pthread_join(tids[j], NULL);
    }

    free(vi);
    free(tids);
    return 0;
}
