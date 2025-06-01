// quatro_threads.c
#include <pthread.h>
#include <stdio.h>

void *t1(void *arg) { printf("Hello from thread 1\n"); return NULL; }
void *t2(void *arg) { printf("Hello from thread 2\n"); return NULL; }
void *t3(void *arg) { printf("Hello from thread 3\n"); return NULL; }
void *t4(void *arg) { printf("Hello from thread 4\n"); return NULL; }

int main(void) {
    pthread_t th[4];

    pthread_create(&th[0], NULL, t1, NULL);
    pthread_create(&th[1], NULL, t2, NULL);
    pthread_create(&th[2], NULL, t3, NULL);
    pthread_create(&th[3], NULL, t4, NULL);

    for (int i = 0; i < 4; i++)
        pthread_join(th[i], NULL);

    return 0;
}
