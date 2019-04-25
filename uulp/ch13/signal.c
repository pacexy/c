#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig(int s)
{
    printf("signal reached\n");
}

void make_sig()
{
    alarm(3);
}

int main()
{
    signal(SIGALRM, sig);
    make_sig();
    sleep(5);

    return 0;
}
