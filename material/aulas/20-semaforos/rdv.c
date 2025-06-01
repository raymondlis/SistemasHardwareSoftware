/*
compile:
gcc rdv.c -o rdv -pthread
*/
#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    sem_t *s1;
    sem_t *s2;
} rdv_args_t;

void *thread1(void *arg) {
    rdv_args_t *r = (rdv_args_t*)arg;
    printf("A\n");

    // TODO: implemente aqui o RDV
    sem_post(r->s1);
    sem_wait(r->s2);

    printf("C\n");

    return NULL;
}

void *thread2(void *arg) {
    rdv_args_t *r = (rdv_args_t*)arg;

    printf("B\n");

    // TODO: implemente aqui o RDV
    sem_post(r->s2);
    sem_wait(r->s1);


    printf("D\n");

    return NULL;
}
int main() {
    // TODO: Crie TODAS as threads. Voce deve utilizar semaforos para sincronizacao.
    sem_t s1,s2;

    // 1) inicializa semáforos em 0
    sem_init(&s1, 0, 0);
    sem_init(&s2, 0, 0);

    rdv_args_t args = {.s1 = &s1, .s2 = &s2};
    // 2) cria as duas threads
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, &args);
    pthread_create(&t2, NULL, thread2, &args);

    // 3) espera elas terminarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // 4) destrói semáforos antes de sair
    sem_destroy(&s1);
    sem_destroy(&s2);
    return 0;
}
