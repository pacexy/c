/* ash2.0 */
/* update:  support any long arglist */
/* update:  give execute() a status */
#include <stdio.h>
#include <stdlib.h>
#include "ash2.h"

int i = 0;
int main()
{
    char **arglist;
    char *cmd;

    sig_mode(0);
    while ((cmd = next_cmd()) != NULL) {
        i = 0;
        /* printf("cmd:%s", cmd); */
        while ((arglist = parse(cmd)) != NULL) {
            execute(arglist);
            freelist(arglist);
        }
        free(cmd);
    }
    return 0;
}
