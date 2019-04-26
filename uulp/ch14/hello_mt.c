#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM 3

int main(void)
{
    pthread_t t1, t2;
    void *print_msg(void *);

    pthread_create(&t1, NULL, print_msg, (void *) "hello");
    pthread_create(&t2, NULL, print_msg, (void *) "world\n");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

void *print_msg(void *v)
{
    char *s = (char *) v;
    int i;

    for (i = 0; i < NUM; i++) {
        printf("%s", s);
        fflush(stdout);
        sleep(1);
    }
    return NULL;
}
