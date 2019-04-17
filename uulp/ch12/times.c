#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>


int make_server_socket(int);
void process_request(int);
void oops(char *);

int main(int argc, char *argv[])
{
    int sockfd, fd;
    int port = atoi(argv[1]);
    sockfd = make_server_socket(port);
    while (1) {
        if ((fd = accept(sockfd, NULL, NULL)) == -1)
            oops("accpet");
        process_request(fd);
        close(fd);
    }
}

void process_request(int fd)
{
    /* time_t t; */
    /* char *s; */
    /* t = time(NULL); */
    /* s = ctime(&t); */
    /* write(fd, s, strlen(s)); */
    int pid;
    if ((pid = fork()) == -1)
        oops("fork");
    else if (pid == 0) {
        dup2(fd, 1);
        close(fd);
        execlp("date", "date", NULL);
        oops("date");
    }
    else
        if (wait(NULL) != pid)
            oops("wait");
}
