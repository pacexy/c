#include <stdio.h>
#include <fcntl.h>

int main()
{
    int c;
    int s;

    s = fcntl(0, F_GETFL);
    s |= O_SYNC;
    if (fcntl(0, F_SETFL, s) == -1)
        perror("fcntl");

    while ((c = getchar()) != EOF)
        putchar(c);
}
