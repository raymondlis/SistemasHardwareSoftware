#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct soma_parcial_args {
    double *vetor;
    int start, end;
    pthread_mutex_t *mutex;
};

double soma = 0;
void *soma_parcial(void *_arg) {
    struct soma_parcial_args *spa = _arg;

    for (int i = spa->start; i < spa->end; i++) {

        pthread_mutex_lock(spa->mutex);
        soma += spa->vetor[i];
        pthread_mutex_unlock(spa->mutex);

    }

    return NULL;
}

int main(int argc, char *argv[]) {
    double *vetor = NULL;
    int n;
    scanf("%d", &n);

    vetor = malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++) {
        scanf("%lf", &vetor[i]);
    }


    /* TODO: criar thread_t e soma_parcial_args aqui */
    pthread_mutex_t soma_mutex;
    pthread_mutex_init(&soma_mutex, NULL);


    pthread_t threads[4];
    struct soma_parcial_args args[4];
    int chunk = n/4;
    for (int i = 0; i < 4; i++) {
        args[i].vetor = vetor;
        args[i].start = i * chunk;
        args[i].end   = (i == 3 ? n : (i+1)*chunk);
        args[i].mutex = &soma_mutex;
        pthread_create(&threads[i], NULL, soma_parcial, &args[i]);
    }
    for (int i = 0; i < 4; i++)
        pthread_join(threads[i], NULL);
    printf("Paralela: %lf\n", soma);

    soma = 0;
    struct soma_parcial_args full = {
        .vetor = vetor,
        .start = 0,
        .end   = n,
        .mutex = &soma_mutex
    };
    soma_parcial(&full);
    printf("Sequencial:               %lf\n", soma);

    // 5) cleanup
    pthread_mutex_destroy(&soma_mutex);
    free(vetor);
    return 0;
}
