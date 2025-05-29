#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int status = 0;

void operacao_lenta() {
    sleep(10);
}


void sigint_handler(int num) {
    status += 1;
    printf("Chamou Ctrl+C; status=%d\n", status);
    operacao_lenta();
    printf("SIGINT: Vou usar status agora! status=%d\n", status);
}

void sigterm_handler(int num) {
    status += 1;
    printf("Recebi SIGTERM; status=%d\n", status);
    operacao_lenta();
    printf("SIGTERM: Vou usar status agora! status=%d\n", status);
}

int main() {
    struct sigaction sa_int, sa_term;

    /* TODO: registar SIGINT aqui. */
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaddset(&sa_int.sa_mask,SIGTERM);
    sigaction(SIGINT, &sa_int, NULL);

    /* TODO: registar SIGTERM aqui. */
    sa_term.sa_handler = sigterm_handler;
    sigemptyset(&sa_term.sa_mask);
    sa_term.sa_flags = 0;
    sigaddset(&sa_term.sa_mask,SIGINT);
    sigaction(SIGTERM, &sa_term, NULL);

    printf("Meu pid: %d\n", getpid());

    while(1) {
        sleep(1);
    }
    return 0;
}
