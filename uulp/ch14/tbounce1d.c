#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <curses.h>
#include <pthread.h>

#define MSG "fuck"
#define BLK "    "

int col;
int row;
int xdir = 0;
int ydir = 0;
int delay = 100;
void *move_img(void *);
void set_mode(void);

int main(void)
{
    pthread_t t;
    int c;

    initscr();
    set_mode();
    clear();

    if (pthread_create(&t, NULL, move_img, NULL) == -1) {
        endwin();
        exit(1);
    }
    while ((c = getchar()) != 'q') {
        switch(c) {
            case 'w': ydir = -1; xdir = 0; break;
            case 's': ydir =  1; xdir = 0; break;
            case 'a': xdir = -1; ydir = 0; break;
            case 'd': xdir =  1; ydir = 0; break;
            default: break;
        }
    }

    endwin();
}

void set_mode(void)
{
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~ICANON;
    t.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &t);
}

#define ROW ws.ws_row
#define COL ws.ws_col

void *move_img(void * v)
{
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    row = ROW/2;
    col = COL/2;

    while (1) {
        move(row, col);
        addstr(MSG);
        refresh();

        usleep(delay * 1000);
        move(row, col);
        addstr(BLK);

        if ((row + 1) == ROW || (row - 1) < 0)
            ydir = -ydir;
        if ((col + strlen(MSG) + 1) == COL || (col - strlen(MSG)) < 1)
            xdir = -xdir;
        row += ydir;
        col += xdir;
    }
}
