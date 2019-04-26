#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <ctype.h>

#define NUM ac-1

int main(int ac, char *av[])
{
    int i;
    int total = 0;
    void *v[NUM];
    pthread_t p[NUM];
    /* char arg[NUM][BUFSIZ]; */

    void *wc(void *);

    for (i = 0; i < NUM; i++) {
        /* sprintf(arg[i], "%d %s", i, av[i+1]); */
        /* pthread_create(&p[i], NULL, wc, (void *) arg[i]); */
        pthread_create(&p[i], NULL, wc, (void *) av[i+1]);
    }

    for (i = 0; i < NUM; i++)
        pthread_join(p[i], &v[i]);

    for (i = 0; i < NUM; i++) {
        total += *(int *) v[i];
        printf("%8d %s\n", *(int *) v[i], av[i+1]);
        free(v[i]);
    }

    printf("%8d total\n", total);
    printf("time used: %.2f\n", (double) clock()/CLOCKS_PER_SEC);

    return 0;
}

void *wc(void *s)
{
    int fd;
    int *count;
    char c;
    char prevc = '\0';
    const char *filename;
    filename = (char *) s;
    count = (int *) malloc(sizeof(int));
    *count = 0;

    if ((fd = open(filename, O_RDONLY)) == -1)
        perror(filename);
    else {
        while (read(fd, &c, 1) == 1) {
            if (isalnum(c) && !isalnum(prevc))
                (*count)++;
            prevc = c;
        }
        close(fd);
    }

    return (void *) count;
}
