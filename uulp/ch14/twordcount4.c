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
struct args *mailbox;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag = PTHREAD_COND_INITIALIZER; 

int main(int ac, char *av[])
{
    int total = 0;
    int reports_in;
    pthread_t p1, p2;
    void *wc(void *);
    struct args arg1, arg2;

    arg1.filename = av[1];
    arg2.filename = av[2];
    arg1.count = 0;
    arg2.count = 0;
    pthread_create(&p1, NULL, wc, (void *) &arg1);
    pthread_create(&p2, NULL, wc, (void *) &arg2);


    for (reports_in = 0; reports_in < 2; reports_in++) {
        printf("MAIN: waiting for flag to go up\n");
        pthread_cond_wait(&flag, &lock);
        printf("MAIN: Wow! flag was raised, I have the lock\n");
        printf("%8d: %s\n", mailbox->count, mailbox->filename);
        total += mailbox->count;
        if (mailbox == &arg1)
            pthread_join(p1, NULL);
        if (mailbox == &arg2)
            pthread_join(p2, NULL);
        mailbox = NULL;
        pthread_cond_signal(&flag);
    }
    printf("%8d total\n", arg1.count + arg2.count);
    sleep(1);
    printf("time used: %.6f\n", (double) clock()/CLOCKS_PER_SEC);

    return 0;
}

void *wc(void *s)
{
    int fd;
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
    printf("COUNT: waiting to get lock\n");
    pthread_mutex_lock(&lock);
    printf("COUNT: have lock, storing data\n");
    if (mailbox != NULL)
        pthread_cond_wait(&flag, &lock);
    mailbox = arg;
    printf("COUNT: raising flag\n");
    pthread_cond_signal(&flag);
    printf("COUNT: unlocking box\n");
    pthread_mutex_unlock(&lock);

    return NULL;
}
