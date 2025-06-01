// soma_global2.c
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct soma_parcial_args {
    double        *vetor;
    int            start, end;
    pthread_mutex_t *mutex;
};

double soma = 0;

void *soma_parcial(void *_arg) {
    struct soma_parcial_args *spa = _arg;
    double local_sum = 0.0;

    // acumula localmente sem tocar no mutex
    for (int i = spa->start; i < spa->end; i++) {
        local_sum += spa->vetor[i];
    }

    // só uma única vez atualiza a soma global
    pthread_mutex_lock(spa->mutex);
    soma += local_sum;
    pthread_mutex_unlock(spa->mutex);

    return NULL;
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Usage: <program> n < n doubles>\n");
        return 1;
    }

    double *vetor = malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++) {
        scanf("%lf", &vetor[i]);
    }

    pthread_mutex_t soma_mutex;
    pthread_mutex_init(&soma_mutex, NULL);

    const int T = 4;
    pthread_t threads[T];
    struct soma_parcial_args args[T];

    int chunk = n / T;
    for (int i = 0; i < T; i++) {
        args[i].vetor = vetor;
        args[i].start = i * chunk;
        args[i].end   = (i == T - 1 ? n : (i + 1) * chunk);
        args[i].mutex = &soma_mutex;
        pthread_create(&threads[i], NULL, soma_parcial, &args[i]);
    }

    for (int i = 0; i < T; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Paralela (reduzido mutex): %lf\n", soma);

    // versão sequencial para comparação
    soma = 0;
    struct soma_parcial_args full = {
        .vetor = vetor,
        .start = 0,
        .end   = n,
        .mutex = &soma_mutex
    };
    soma_parcial(&full);
    printf("Sequencial:               %lf\n", soma);

    pthread_mutex_destroy(&soma_mutex);
    free(vetor);
    return 0;
}
