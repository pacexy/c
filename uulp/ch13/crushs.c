#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define RECLAIM_INTERVAL 60
#define TICKLEN 16
#define MAXNUM  16
void ticket_reclaim(int s)
{
    int i;
    char ticket[TICKLEN];

    for (i = 0; i < MAXNUM; i++) {
        if (ticket_list[i] != 0 && (kill(ticket_list[i], 0) == -1) && errno == ESRCH) {
            sprintf(ticket, "%d.%d", ticket_list[i], i);
            narrate("tick", ticket, NULL);
            ticket_list[i] = 0;
            rest++;
        }
    }
    alarm(RECLAIM_INTERVAL);
}
