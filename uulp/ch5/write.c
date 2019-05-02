#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int ac, char *av[])
{
    int fd;
    char buf[BUFSIZ];

    if (ac != 2) {
        fprintf(stderr, "Usage:fuck you\n");
        exit(1);
    }

    if ((fd = open(av[1], O_WRONLY)) == -1) {
        perror("open");
        exit(1);
    }

    while (fgets(buf, BUFSIZ, stdin) != NULL)
        if (write(fd, buf, BUFSIZ) == -1)
            break;

    close(fd);
}
