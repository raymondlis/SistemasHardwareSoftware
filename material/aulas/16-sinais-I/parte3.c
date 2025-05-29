// parte2.c
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define _POSIX_C_SOURCE 200112L
#include <string.h>

int main(void) {
    pid_t filho = fork();

    if (filho == 0) {
        // Estamos no filho
        printf("Eu sou o filho, meu PID = %d\n", getpid());
        // loop infinito
        for (;;) pause();
    } else {
        // Estamos no pai: espera o término do filho

        printf("Pai (PID = %d) vai esperar 10s antes de matar o filho %d\n",
               getpid(), filho);
        sleep(10);

        kill(filho, SIGKILL);
        printf("Pai: enviado SIGKILL ao filho %d\n", filho);
        
        int status;
        pid_t w = wait(&status);
        printf("Pai: criança com PID %d terminou (wait retornou %d)\n",
               filho, (int)w);

        printf("wait() retornou PID = %d (esperado %d)\n", w, filho);
        printf("WIFEXITED(status)   = %d\n", WIFEXITED(status));
        printf("WIFSIGNALED(status) = %d\n", WIFSIGNALED(status));
        // Só faz sentido chamar WTERMSIG se WIFSIGNALED for 1:
        if (WIFSIGNALED(status)) {
            printf("WTERMSIG(status)    = %d\n", WTERMSIG(status));
        
            /*int sig = WTERMSIG(status);
            printf("WTERMSIG(status) strsignal    = %d (%s)\n",
                   sig, strsignal(sig));*/
        }
        
        if (w == filho) {
            if (WIFSIGNALED(status)) {
                printf("Filho com PID %d terminou por sinal %d\n",
                       filho, WTERMSIG(status));
            } else if (WIFEXITED(status)) {
                printf("Filho com PID %d terminou normalmente com código %d\n",
                       filho, WEXITSTATUS(status));
            }
        }
    }
    return 0;
}
