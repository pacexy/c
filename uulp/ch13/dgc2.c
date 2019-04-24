#include <stdlib.h>

void setup(void);
int get_ticket(void);
void do_regular_work(void);
void release_ticket(void);
void finish(void);

int main()
{
    setup();

    if (get_ticket() != 0)
        exit(1);
    do_regular_work();
    release_ticket();

    finish();
}
