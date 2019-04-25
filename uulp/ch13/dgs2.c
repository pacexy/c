#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define RECLAIM_INTERVAL 10

void setup(void);
void transaction(void);
void finish();
void ticket_reclaim(int);
int main(void)
{
    setup();

    signal(SIGALRM, ticket_reclaim);
    alarm(RECLAIM_INTERVAL);

    while (1) {
        transaction();
    }

    finish();
    return 0;
}
