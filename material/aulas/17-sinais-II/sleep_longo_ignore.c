/* sleep_longo_ignore.c */
#define _POSIX_C_SOURCE 200112L
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main(void) {
    /* ignora SIGTERM */
    struct sigaction st = {0};
    st.sa_handler = SIG_IGN;
    sigemptyset(&st.sa_mask);
    st.sa_flags = 0;
    sigaction(SIGTERM, &st, NULL);

    printf("Meu PID: %d\n", getpid());
    printf("Tempo restante: %u\n", sleep(20));
    return 0;
}
