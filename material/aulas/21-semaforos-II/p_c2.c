// pc2.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

typedef struct {
    int     buffer[BUFFER_SIZE];
    int     in, out;
    pthread_mutex_t mutex;
    sem_t   vagas, itens;
} pc_args_t;

int produzir_item() {
    return rand()%1000;
}

void *produtor(void *_a) {
    pc_args_t *a = _a;
    while (1) {
        int item = produzir_item();
        int s = rand()%6;
        sleep(s);

        sem_wait(&a->vagas);

        pthread_mutex_lock(&a->mutex);
        a->buffer[a->in] = item;
        a->in = (a->in+1)%BUFFER_SIZE;
        int vagas_sobrando;
        sem_getvalue(&a->vagas, &vagas_sobrando);
        printf("[Produtor]  inseriu %3d (sleep %ds) — vagas sobrando: %d\n",
               item, s, vagas_sobrando);
        pthread_mutex_unlock(&a->mutex);

        sem_post(&a->itens);
    }
    return NULL;
}

void *consumidor(void *_a) {
    pc_args_t *a = _a;
    while (1) {
        sem_wait(&a->itens);

        pthread_mutex_lock(&a->mutex);
        int item = a->buffer[a->out];
        a->out = (a->out+1)%BUFFER_SIZE;
        int itens_disp;
        sem_getvalue(&a->itens, &itens_disp);
        pthread_mutex_unlock(&a->mutex);

        sem_post(&a->vagas);

        int p = (rand()%2)+1;
        sleep(p);
        printf("[Consumidor] removeu %3d (proc %ds) — itens sobrando: %d\n",
               item, p, itens_disp);
        fflush(stdout);
    }
    return NULL;
}

int main(void) {
    srand(time(NULL));
    pc_args_t args = {
        .in = 0, .out = 0,
        .mutex = PTHREAD_MUTEX_INITIALIZER
    };
    sem_init(&args.vagas, 0, BUFFER_SIZE);
    sem_init(&args.itens, 0, 0);

    pthread_t p, c;
    pthread_create(&p, NULL, produtor,   &args);
    pthread_create(&c, NULL, consumidor, &args);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    return 0;
}
