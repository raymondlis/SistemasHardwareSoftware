// pc_mn.c
// Compile com:
//   gcc pc_mn.c -o pc_mn -pthread

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5   // tamanho do buffer circular
#define PRODUCERS   3   // número de threads produtoras
#define CONSUMERS   4   // número de threads consumidoras

/* Estrutura de dados compartilhada */
typedef struct {
    int     buffer[BUFFER_SIZE];
    int     in, out;
    pthread_mutex_t mutex;
    sem_t   vagas;    // semáforo de vagas livres
    sem_t   itens;    // semáforo de itens disponíveis
} pc_shared_t;

/* Argumento para cada thread */
typedef struct {
    int          id;
    pc_shared_t *shared;
} thread_arg_t;

/* Gera um item qualquer */
static int produzir_item(int producer_id) {
    return (producer_id * 1000) + (rand()%1000);
}

/* Código da thread produtora */
void *produtor(void *arg_) {
    thread_arg_t *arg = (thread_arg_t*)arg_;
    pc_shared_t  *sh  = arg->shared;
    int id = arg->id;

    while (1) {
        /* Simula tempo de produção: 0–2 segundos */
        int t = rand() % 3;
        sleep(t);

        int item = produzir_item(id);

        /* Espera por vaga livre */
        sem_wait(&sh->vagas);

        /* Seção crítica: insere no buffer */
        pthread_mutex_lock(&sh->mutex);
            sh->buffer[sh->in] = item;
            sh->in = (sh->in + 1) % BUFFER_SIZE;
            int vagas_sobrando;
            sem_getvalue(&sh->vagas, &vagas_sobrando);
            printf("[P% d] produziu %4d (sleep %ds) — vagas: %d\n",
                   id, item, t, vagas_sobrando);
        pthread_mutex_unlock(&sh->mutex);

        /* Sinaliza que há um novo item */
        sem_post(&sh->itens);
    }
    return NULL;
}

/* Código da thread consumidora */
void *consumidor(void *arg_) {
    thread_arg_t *arg = (thread_arg_t*)arg_;
    pc_shared_t  *sh  = arg->shared;
    int id = arg->id;

    while (1) {
        /* Espera por item disponível */
        sem_wait(&sh->itens);

        /* Seção crítica: retira do buffer */
        pthread_mutex_lock(&sh->mutex);
            int item = sh->buffer[sh->out];
            sh->out = (sh->out + 1) % BUFFER_SIZE;
            int itens_disp;
            sem_getvalue(&sh->itens, &itens_disp);
        pthread_mutex_unlock(&sh->mutex);

        /* Sinaliza que liberou uma vaga */
        sem_post(&sh->vagas);

        /* Simula tempo de consumo: 1–3 segundos */
        int t = 1 + (rand() % 3);
        sleep(t);
        printf("[C% d] consumiu %4d (proc %ds) — itens: %d\n",
               id, item, t, itens_disp);
        fflush(stdout);
    }
    return NULL;
}

int main(void) {
    srand(time(NULL));

    pc_shared_t shared = {
        .in = 0,
        .out = 0,
        .mutex = PTHREAD_MUTEX_INITIALIZER
    };
    sem_init(&shared.vagas, 0, BUFFER_SIZE);
    sem_init(&shared.itens, 0, 0);

    /* Arrays de argumentos e threads */
    pthread_t    prod_th[PRODUCERS], cons_th[CONSUMERS];
    thread_arg_t prod_args[PRODUCERS], cons_args[CONSUMERS];

    /* Cria threads produtoras */
    for (int i = 0; i < PRODUCERS; i++) {
        prod_args[i].id     = i;
        prod_args[i].shared = &shared;
        pthread_create(&prod_th[i], NULL, produtor, &prod_args[i]);
    }
    /* Cria threads consumidoras */
    for (int i = 0; i < CONSUMERS; i++) {
        cons_args[i].id     = i;
        cons_args[i].shared = &shared;
        pthread_create(&cons_th[i], NULL, consumidor, &cons_args[i]);
    }

    /* Espera (join) — neste exemplo rodamos em loop infinito */
    for (int i = 0; i < PRODUCERS ; i++)
        pthread_join(prod_th[i], NULL);
    for (int i = 0; i < CONSUMERS; i++)
        pthread_join(cons_th[i], NULL);

    /* Nunca alcançado, mas por completude */
    sem_destroy(&shared.vagas);
    sem_destroy(&shared.itens);
    pthread_mutex_destroy(&shared.mutex);
    return 0;
}
