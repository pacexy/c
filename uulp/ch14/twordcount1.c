#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <ctype.h>

int count;

int main(int ac, char *av[])
{
    /* void *c1, *c2; */
    pthread_t p1, p2;
    void *wc(void *);

    pthread_create(&p1, NULL, wc, (void *) av[1]);
    pthread_create(&p2, NULL, wc, (void *) av[2]);

    /* pthread_join(p1, &c1); */
    /* pthread_join(p2, &c2); */
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("%8d total\n", count);
    sleep(1);
    printf("time used: %.6f\n", (double) clock()/CLOCKS_PER_SEC);

    return 0;
}

void *wc(void *s)
{
    int fd;
    /* static int count = 0; */
    char c;
    char prevc = '\0';
    const char *filename;
    filename = (char *) s;

    if ((fd = open(filename, O_RDONLY)) == -1)
        perror(filename);
    else {
        while (read(fd, &c, 1) == 1) {
            if (isalnum(c) && !isalnum(prevc)) {
                count++;
            }
            prevc = c;
        }
        close(fd);
    }

    /* return (void *) &count; */
    return NULL;
}
