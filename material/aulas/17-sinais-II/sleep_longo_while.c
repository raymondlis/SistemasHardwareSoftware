/* sleep_longo_while.c */
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void handler(int num) {
    printf("Interrompi o sleep!\n");
}

int main(int argc, char *argv[]) {
    struct sigaction st;
    st.sa_handler = handler;
    st.sa_flags   = SA_RESTART;
    sigemptyset(&st.sa_mask);
    sigaction(SIGTERM, &st, NULL);

    printf("Meu PID: %d\n", getpid());

    unsigned int remaining = 20;
    while (remaining > 0) {
        remaining = sleep(remaining);
        printf("Ainda faltam %u segundos\n", remaining);
    }
    printf("Dormiu por 20 segundos completos.\n");
    return 0;
}
