#include <stdio.h>
#include <sys/ioctl.h>

int main()
{
    struct winsize ws;

    if (ioctl(0, TIOCGWINSZ, &ws) != -1) {
        printf("%d x %d\n", ws.ws_row, ws.ws_col);
        printf("%d x %d\n", ws.ws_xpixel, ws.ws_ypixel);
    }
}
