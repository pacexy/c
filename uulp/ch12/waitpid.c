#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{
    int pid;

    printf("i am parent, i will create a child\n");
    if ((pid = fork()) == -1) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) {
        printf("i am child, i will sleep\n");
        sleep(5);
        printf("i am child, i will exit\n");
        exit(0);
    }
    else {
        printf("i am parent, i will waitpid\n");
        while (waitpid(-1, NULL, WNOHANG) == 0) {
            printf("child is running\n");
            sleep(1);
        }
        printf("child is exit and be recycling\n");
    }
}
