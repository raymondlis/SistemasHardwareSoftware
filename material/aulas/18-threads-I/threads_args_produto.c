// threads_args_produto.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int x;
    int y;
    int produto;    // campo para a thread guardar o resultado
} DoisInts;

void *tarefa_calc(void *arg) {
    DoisInts *data = (DoisInts *)arg;
    data->produto = data->x * data->y;
    return NULL;
}

int main(void) {
    const int N = 4;
    pthread_t *tids = malloc(N * sizeof(pthread_t));
    DoisInts  *arr  = malloc(N * sizeof(DoisInts));
    if (!tids || !arr) {
        perror("malloc");
        return 1;
    }

    // cria as threads, passando x=j, y=j*10
    for (int j = 0; j < N; j++) {
        arr[j].x = j;
        arr[j].y = j * 10;
        int err = pthread_create(&tids[j], NULL, tarefa_calc, &arr[j]);
        if (err) {
            fprintf(stderr, "Erro pthread_create: %d\n", err);
            return 1;
        }
    }

    // espera e, após cada join, imprime o produto calculado
    for (int j = 0; j < N; j++) {
        pthread_join(tids[j], NULL);
        printf("Thread %d: %d * %d = %d\n",
               j, arr[j].x, arr[j].y, arr[j].produto);
    }

    free(arr);
    free(tids);
    return 0;
}
