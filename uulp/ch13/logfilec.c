#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKNAME "/"

void oops(const char *s)
{
    perror(s);
    exit(1);
}

int main(int ac, char *av[])
{
    int sockfd;
    struct sockaddr_un sa;
    socklen_t sl;
    char sockname[] = SOCKNAME;

    if ((sockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) == -1)
        oops("socket");

    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, sockname);
    sl = sizeof(sockname) + sizeof(sa.sun_family);

    if (sendto(sockfd, av[1], strlen(av[1]), 0, (struct sockaddr *) &sa, sl) == -1)
        oops("sendto");
}
