#include <stdio.h>

int main()
{
    int c;
    while ((c = getc(stdin)) != EOF)
        putchar(c);
}
