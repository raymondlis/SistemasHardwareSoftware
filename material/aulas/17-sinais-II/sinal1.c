#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

int contador = 0;

/* Este é o nosso “handler” de SIGINT. Toda vez
 * que o usuário aperta Ctrl+C, o kernel interrompe
 * o sleep(1) e invoca esta função, passando o número
 * do sinal (aqui sempre  SIGINT == 2). */
void sig_handler(int signo) {
    (void) signo;            // não usamos diretamente
    contador++;
    printf("Chamou Ctrl+C %d/3\n", contador);

    if (contador == 3) {
        /* Ao invés de chamar exit(0) (que faz o shell
         * pensar que o programa saiu normalmente), nós:
         *
         * 1) Restauramos o comportamento padrão de SIGINT
         *    (SIG_DFL), fazendo com que o próximo SIGINT
         *    mate o processo de verdade;
         * 2) Em seguida, “re-enviamos” para nós mesmos
         *    o SIGINT, usando raise(). Assim, vai valer
         *    a ação padrão, e o shell verá que houve
         *    uma interrupção por sinal. */
        struct sigaction sa;
        sa.sa_handler = SIG_DFL;        // volta ao default
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, NULL);   // reinstala default
        kill(getpid(), SIGINT);

    }
}

void sigtstp_handler(int signo) {
    (void)signo;
    printf("Recebi SIGTSTP (Ctrl+Z) — vou parar agora\n");
    /* Restaura ação-padrão para SIGTSTP e reenvia */
    struct sigaction sa;
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTSTP, &sa, NULL);
    raise(SIGTSTP);
}

/* Handler para “fg”     (SIGCONT) — imprime ao continuar */
void sigcont_handler(int signo) {
    (void)signo;
    printf("Continuando!\n");
}

int main(void) {
    struct sigaction sa;

    /* Prepara a struct:
     * - sa_handler : apontador para a nossa função de callback
     * - sa_mask    : conjunto de sinais a bloquear enquanto
     *                estamos dentro do handler (aqui nenhum)
     * - sa_flags   : 0 por ora, mas poderíamos usar SA_RESTART,
     *                SA_NOCLDSTOP, etc. */
    sa.sa_handler = sig_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    /* Registra o handler para SIGINT */
    sigaction(SIGINT, &sa, NULL);
    
    /* --- SIGTSTP --- */
    sa.sa_handler = sigtstp_handler;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTSTP, &sa, NULL);

    /* --- SIGCONT --- */
    sa.sa_handler = sigcont_handler;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCONT, &sa, NULL);

    printf("Meu PID é %d.\n", getpid());

    /* Loop infinito para o programa continuar vivo,
     * dormindo 1 segundo por vez até receber sinais. */
    while (1) {
        sleep(1);
    }
    return 0;
}
