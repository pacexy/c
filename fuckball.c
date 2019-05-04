#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RED  1
#define BLUE 0
#define BOXNUM 3
#define BALLNUM 2
struct box {
    int ball[BALLNUM];
};

int main()
{
    int i;
    int whichbox;
    int whichball;
    /* int count[3] = {0}; */
    int total, success;
    struct box boxes[BOXNUM];

    total = success = 0;

    boxes[0].ball[0] = RED;
    boxes[0].ball[1] = RED;

    boxes[1].ball[0] = BLUE;
    boxes[1].ball[1] = BLUE;

    boxes[2].ball[0] = RED;
    boxes[2].ball[1] = BLUE;

    srand(time(NULL));
    for (i = 0; i < 200; i++) {
        whichbox = rand() % BOXNUM;
        whichball = rand() % BALLNUM;
        if (boxes[whichbox].ball[whichball] == RED) {
            total++;
            for (j = 0; j < BOXNUM; j++) {
                if (j != whichbox) {
                    for (k = 0; k <BALLNUM; k++)
                        if (boxes[j].ball[k] == RED) {
                            boxes[j].ball[k] == BLUE;
                        }

                }
            }
            if (boxes[whichbox].ball[!whichball] == RED)
                success++;
        }
        /* count[whichbox]++; */
    }

        /* printf("box[%d]: %d\n", i, count[i]); */
    printf("total:%d success:%d\n", total, success);
}
