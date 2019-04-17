#include <stdio.h>
#include <stdlib.h>
#include "ash.h"

int i = 0;
int main()
{
    char **arglist;
    char cmd[BUFSIZ];

    sig_mode(0);
    while (next_cmd(cmd)) {
        i = 0;
        /* printf("cmd:%s", cmd); */
        while (1) {
            if((arglist = parse(cmd)) != NULL)
                execute(arglist);
            else
                break;
        }
    }
    freelist(arglist);
    return 0;
}
