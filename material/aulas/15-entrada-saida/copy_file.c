/* copy_file.c */
/* POSIX headers */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
/* Standard C */
#include <stdio.h>

int main(int argc, char *argv[]) {
    char buf[1];
    int fd_src, fd_dst;

    /* recebe nomes via argv */
    scanf("%s %s", argv[1], argv[2]);

    fd_src = open(argv[1], O_RDONLY);
    fd_dst = open(argv[2], O_WRONLY | O_CREAT, 0700);

    /* copia byte a byte até EOF */
    while (read(fd_src, buf, 1) > 0) {
        write(fd_dst, buf, 1);
    }   

    close(fd_src);
    close(fd_dst);
    return 0;
}
