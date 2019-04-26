#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <ctype.h>

struct args {
    char *filename;
    int count;
};

int main(int ac, char *av[])
{
    /* void *c1, *c2; */
    int total;
    pthread_t p1, p2;
    void *wc(void *);
    struct args arg1, arg2;

    arg1.filename = av[1];
    arg2.filename = av[2];
    arg1.count = 0;
    arg2.count = 0;
    pthread_create(&p1, NULL, wc, (void *) &arg1);
    pthread_create(&p2, NULL, wc, (void *) &arg2);

    /* pthread_join(p1, &c1); */
    /* pthread_join(p2, &c2); */
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("%8d %s\n", arg1.count, arg1.filename);
    printf("%8d %s\n", arg2.count, arg2.filename);
    printf("%8d total\n", arg1.count + arg2.count);
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
    struct args *arg = (struct args *)s;

    if ((fd = open(arg->filename, O_RDONLY)) == -1)
        perror(arg->filename);
    else {
        while (read(fd, &c, 1) == 1) {
            if (isalnum(c) && !isalnum(prevc)) {
                arg->count++;
            }
            prevc = c;
        }
        close(fd);
    }

    /* return (void *) &count; */
    return NULL;
}
