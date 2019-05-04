#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RED  1
#define BLUE 0
struct box {
    int ball[2];
};

int main()
{
    int i;
    int whichbox;
    int whichball;
    /* int count[3] = {0}; */
    int total, success;
    struct box boxes[3];

    total = success = 0;

    boxes[0].ball[0] = RED;
    boxes[0].ball[1] = RED;

    boxes[1].ball[0] = BLUE;
    boxes[1].ball[1] = BLUE;

    boxes[2].ball[0] = RED;
    boxes[2].ball[1] = BLUE;

    srand(time(NULL));
    for (i = 0; i < 200; i++) {
        whichbox = rand() % 3;
        whichball = rand() % 2;
        if (boxes[whichbox].ball[whichball] == RED) {
            total++;
            if (boxes[whichbox].ball[!whichball] == RED)
                success++;
        }
        /* count[whichbox]++; */
    }

        /* printf("box[%d]: %d\n", i, count[i]); */
    printf("total:%d success:%d\n", total, success);
}
