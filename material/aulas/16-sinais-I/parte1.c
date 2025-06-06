#define _POSIX_C_SOURCE 200112L
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    pid_t filho;

    filho = fork();
    if (filho == 0) {
        int i = 1/0;
        printf("Divisão por zero!\n");
    }  else {
        // código do pai
        int status;
        pid_t w = wait(&status);

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