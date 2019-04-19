#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

void oops(char *s)
{
    perror(s);
    exit(1);
}
int main(int argc, char *argv[])
{
    int fd;
    int pid;
    if ((fd = creat(argv[2], S_IRUSR | S_IWUSR)) == -1)
        oops("open");
    dup2(fd, 1);
    close(fd);

    if ((pid = fork()) == -1)
        oops("fork");
    else if (pid == 0) {
        execlp("cat", "cat", argv[1], NULL);
        oops("execlp");
    }
    else 
        wait(NULL);
}

