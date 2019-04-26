#include <stdio.h>
#include <unistd.h>

#define NUM 3

int main(void)
{
    void print_msg(const char *);

    print_msg("hello");
    print_msg("world\n");
}

void print_msg(const char *s)
{
    int i;

    for (i = 0; i < NUM; i++) {
        printf("%s", s);
        fflush(stdout);
        sleep(1);
    }
}
