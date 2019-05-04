#include <stdio.h>
#include <curses.h>
int main()
{
    int c;

    while ((c = getch()) != 'q')
        putchar(c);

    return 0;
}
