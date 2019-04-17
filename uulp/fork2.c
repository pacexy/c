#include <stdio.h>
#include <unistd.h>

int main()
{
    int  pid;
    pid = fork();
    if (pid == -1)
        perror("fork");
    else if (pid == 0)
        printf("I am child, my pid is %d\n", getpid());
    else
        printf("I am parent, my child is %d\n", pid);
}
