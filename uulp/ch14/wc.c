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
    int n[NUM];
    int total = 0;
    int wc(char *);

    for (i = 0; i < NUM; i++) {
        n[i] = wc(av[i+1]);
        total += n[i];
        printf("%8d %s\n", n[i], av[i+1]);
    }

    printf("%8d total\n", total);
    printf("time used: %.2f\n", (double) clock()/CLOCKS_PER_SEC);

    return 0;
}

int wc(char *filename)
{
    int fd;
    int count = 0;
    char c;
    char prevc = '\0';

    if ((fd = open(filename, O_RDONLY)) == -1)
        perror(filename);
    else {
        while (read(fd, &c, 1) == 1) {
            if (isalnum(c) && !isalnum(prevc))
                count++;
            prevc = c;
        }
        close(fd);
    }

    return  count;
}
