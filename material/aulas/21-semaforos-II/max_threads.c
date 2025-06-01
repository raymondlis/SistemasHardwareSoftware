#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10
#define MAX 2

typedef struct {
    sem_t *sem;
} arg_thread_t;


void *faz_algo(void *arg_) {
    arg_thread_t *arg = arg_;

    // Wait no semáforo: solicita permissão para executar
    sem_wait(arg->sem);

    printf("Thread executando!\n");

    // simula tarefa
    while (random() > 100);

    printf("Thread vai finalizar!\n");

    // Post no semáforo: avisa que terminou de executar
    sem_post(arg->sem);

    return NULL;
}

int main() {

    // Configura semáforo
    sem_t semaph;

    // Inicializa semáforo com o número máximo de threads executando simultaneamente
    sem_init(&semaph, 0, MAX); 

    // Configura argumentos
    arg_thread_t *arg = malloc(sizeof(arg_thread_t));
    arg->sem = &semaph;

    pthread_t *tid = malloc(sizeof(pthread_t) * N);
    // Cria todas as threads (o semáforo irá controlar a execução)
    for (int i=0; i<N; i++){
        pthread_create(&tid[i], NULL, faz_algo, arg);
    }

    // espere pelas threads
    for (int i=0; i<N; i++){
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&semaph);

    free(tid);
    free(arg);

    return 0;
}