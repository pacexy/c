#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <ctype.h>

int fnum;
#define TNUM 16
#define FNUM fnum
#define FPT FNUM/TNUM

int main(int ac, char *av[])
{
    fnum = ac-1;
    int i;
    int total = 0;
    /* void *v[FNUM]; */
    pthread_t p[TNUM];
    int *v;
    void *b[TNUM];
    v = (int *) malloc(FNUM * sizeof(int));
    /* char arg[NUM][BUFSIZ]; */
    char **block = av;

    struct timeval start;
    struct timeval end;

    void *wc(void *);

    gettimeofday(&start, NULL);
    for (i = 0; i < TNUM; i++) {
        /* sprintf(arg[i], "%d %s", i, av[i+1]); */
        /* pthread_create(&p[i], NULL, wc, (void *) arg[i]); */
        pthread_create(&p[i], NULL, wc, (void *) block);
        block += FPT;
    }

    for (i = 0; i < TNUM; i++)
        pthread_join(p[i], &b[i]);

    for (i = 0; i < TNUM; i++) {
        total += *(int *) b[i];
        /* printf("%8d block[%d]\n", *(int *) b[i], i); */
        free(b[i]);
    }

    gettimeofday(&end, NULL);
    printf("%8d total\n", total);
    /* printf("time used: %.2f\n", (double) clock()/CLOCKS_PER_SEC/TNUM); */
    printf("time used: %.3f", (double) (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/CLOCKS_PER_SEC);

    return 0;
}

void *wc(void *s)
{
    int i = 0;
    int fd;
    int count[FPT];
    int *block_total;
    char c;
    char prevc = '\0';
    char **block = (char **) s;
    const char *filename;

    block_total = (int *) malloc(sizeof(int));
    *block_total = 0;

    for (i = 0; i < FPT && (filename = block[i]) != NULL; i++) {
        /* printf("%s\n", filename); */
        if ((fd = open(filename, O_RDONLY)) == -1)
            perror(filename);
        else {
            while (read(fd, &c, 1) == 1) {
                if (isalnum(c) && !isalnum(prevc))
                    count[i]++;
                prevc = c;
            }
            *block_total += count[i];
            printf("%8d %s\n", count[i], filename);
            close(fd);
        }
    }

    return (void *) block_total;
}
