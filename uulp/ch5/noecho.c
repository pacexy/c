#include <stdio.h>
#include <termios.h>

int main()
{
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &t);

    while (getchar() != 'Q');
}
