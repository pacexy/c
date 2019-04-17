#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
int main() 
{
    int pid;
    if ((pid = fork()) == 0) {
        printf("i am 1, i will sleep\n");
        sleep(5);
        printf("i am 1, i will exit\n");
        exit(0);
    }
    else if ((pid = fork()) == 0) {
        printf("i am 1, i will sleep\n");
        sleep(5);
        printf("i am 1, i will exit\n");
        exit(0);
    }
    else if ((pid = fork()) == 0) {
        printf("i am 1, i will sleep\n");
        sleep(5);
        printf("i am 1, i will exit\n");
        exit(0);
    }
}
