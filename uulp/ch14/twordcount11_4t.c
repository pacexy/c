#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <ctype.h>

int fnum;
#define TNUM 8
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

    void *wc(void *);

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
        printf("%8d block[%d]\n", *(int *) b[i], i);
        free(b[i]);
    }

    printf("%8d total\n", total);
    printf("time used: %.2f\n", (double) clock()/CLOCKS_PER_SEC/TNUM);

    return 0;
}

void *wc(void *s)
{
    int i = 0;
    int fd;
    int *count;
    char c;
    char prevc = '\0';
    char **block = (char **) s;
    const char *filename;

    count = (int *) malloc(sizeof(int));
    *count = 0;

    for (i = 0; i < FPT && (filename = block[i]) != NULL; i++) {
        /* printf("%s\n", filename); */
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
    }

    return (void *) count;
}
