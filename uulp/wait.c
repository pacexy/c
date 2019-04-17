#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define DELAY 10
void child_p(int);
void parent_p(pid_t);
int cpid;
int main()
{
    printf("before: parent pid is %d\n", getpid());
    cpid = fork();
    if (cpid == -1)
        perror("fork");
    else if (cpid == 0)
        child_p(DELAY);
    else
        parent_p(cpid);
}

void child_p(int delay)
{
    printf("child %d here, will sleep for %d secs.\n", getpid(), delay);
    sleep(delay);
    exit(printf("child done, about to exit.\n"));
}

void parent_p(pid_t child)
{
    pid_t w;
    int stat;
    int high_8, low_7, bit_7;
    w = wait(&stat);
    printf("do waiting for %d, returned: %d\n", child, w);

    high_8 = stat >> 8;
    low_7 = stat & 0x7F;
    bit_7 = stat & 0x80;

    printf("status: exit = %d, sig = %d, core = %d\n", high_8, low_7, bit_7);


}
