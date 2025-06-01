// threads_args_2.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int x;
    int y;
} DoisInts;

void *tarefa_print_xy(void *arg) {
    DoisInts *data = (DoisInts *)arg;
    printf("Thread %lu recebeu x = %d, y = %d\n",
           (unsigned long)pthread_self(),
           data->x, data->y);
    return NULL;
}

int main(void) {
    const int N = 4;

    // 1) aloca um array de structs DoisInts em heap
    DoisInts *arr = malloc(N * sizeof(DoisInts));
    pthread_t *tids = malloc(N * sizeof(pthread_t));

    // 2) preenche cada struct e cria uma thread passando &arr[j]
    for (int j = 0; j < N; j++) {
        arr[j].x = j;
        arr[j].y = j * 10;
        pthread_create(&tids[j], NULL, tarefa_print_xy, &arr[j]);
    }

    // 3) espera todas as threads
    for (int j = 0; j < N; j++) {
        pthread_join(tids[j], NULL);
    }

    free(arr);
    free(tids);
    return 0;
}

/*
// threads_args_2.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int x;
    int y;
} DoisInts;

void *tarefa_print_xy(void *arg) {
    DoisInts *data = (DoisInts *)arg;
    printf("Thread %lu recebeu x = %d, y = %d\n",
           (unsigned long)pthread_self(),
           data->x, data->y);
    return NULL;
}

int main(void) {
    const int N = 4;

    // 1) aloca um array de structs DoisInts em heap
    DoisInts *arr = malloc(N * sizeof(DoisInts));
    pthread_t *tids = malloc(N * sizeof(pthread_t));
    if (!arr || !tids) {
        perror("malloc");
        return 1;
    }

    // 2) preenche cada struct e cria uma thread passando &arr[j]
    for (int j = 0; j < N; j++) {
        arr[j].x = j;
        arr[j].y = j * 10;
        int err = pthread_create(&tids[j], NULL, tarefa_print_xy, &arr[j]);
        if (err) {
            fprintf(stderr, "pthread_create erro: %d\n", err);
            return 1;
        }
    }

    // 3) espera todas as threads
    for (int j = 0; j < N; j++) {
        pthread_join(tids[j], NULL);
    }

    free(arr);
    free(tids);
    return 0;
}

*/