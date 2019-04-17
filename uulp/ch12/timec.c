#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int connect_to_server(char *, int);
void talk_to_server(int);

int main(int argc, char *argv[])
{
    int sockfd;
    sockfd = connect_to_server(argv[1], atoi(argv[2]));
    talk_to_server(sockfd);
    close(sockfd);
}

void talk_to_server(int fd)
{
    int len;
    char buf[BUFSIZ];
    while ((len = read(fd, buf, BUFSIZ)) > 0)
        write(1, buf, len);
}
