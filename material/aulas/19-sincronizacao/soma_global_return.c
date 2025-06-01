// soma_global_return.c
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct soma_parcial_args {
    double *vetor;
    int     start, end;
};

void *soma_parcial(void *_arg) {
    struct soma_parcial_args *spa = _arg;
    double local_sum = 0.0;
    for (int i = spa->start; i < spa->end; i++) {
        local_sum += spa->vetor[i];
    }
    // retorna o resultado via heap
    double *res = malloc(sizeof(double));
    if (!res) perror("malloc");
    *res = local_sum;
    return res;
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Uso: <programa> <n> seguido de n valores de ponto flutuante\n");
        return 1;
    }

    // aloca e lê o vetor
    double *vetor = malloc(sizeof(double) * n);
    if (!vetor) { perror("malloc"); return 1; }
    for (int i = 0; i < n; i++) {
        scanf("%lf", &vetor[i]);
    }

    // paraleliza em 4 threads
    const int T = 4;
    pthread_t threads[T];
    struct soma_parcial_args args[T];

    int chunk = n / T;
    for (int i = 0; i < T; i++) {
        args[i].vetor = vetor;
        args[i].start = i * chunk;
        args[i].end   = (i == T - 1 ? n : (i + 1) * chunk);
        pthread_create(&threads[i], NULL, soma_parcial, &args[i]);
    }

    // coleta os resultados
    double total = 0.0;
    for (int i = 0; i < T; i++) {
        void *ret;
        pthread_join(threads[i], &ret);
        double partial = *(double *)ret;
        free(ret);
        total += partial;
    }

    printf("Paralela (return): %lf\n", total);

    // soma sequencial para conferência
    double seq = 0.0;
    for (int i = 0; i < n; i++) {
        seq += vetor[i];
    }
    printf("Sequencial:       %lf\n", seq);

    free(vetor);
    return 0;
}
