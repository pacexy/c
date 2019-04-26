#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 5

int count;

int main()
{
    int i;
    pthread_t t;
    void *print_count(void *);

    pthread_create(&t, NULL, print_count, NULL);
    for (i = 0; i < MAX; i++) {
        sleep(1);
        count++;
    }

    pthread_join(t, NULL);
    return 0;
}

void *print_count(void *v)
{
    int i;
    for (i = 0; i < MAX; i++) {
        printf("%d\n", count);
        sleep(1);
    }

    return NULL;
}
