#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define SOCKNAME "tmp/logfilesock"
#define MSGLEN 512

void oops(const char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_un sa;
    char sockname[] = SOCKNAME;
    char msg[MSGLEN];
    int sockfd;
    int sl;
    int len;
    int msgnum = 0;
    time_t now;
    char *timestr;

    sockfd = socket(PF_UNIX, SOCK_DGRAM, 0);

    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, sockname);
    sl = sizeof(sockname) + sizeof(sa.sun_family);

    if (bind(sockfd, (struct sockaddr *) &sa, sl) == -1)
        oops("bind");

    while (1) {
        if ((len = read(sockfd, msg, MSGLEN)) == -1)
            oops("read");

        now = time(NULL);
        timestr = ctime(&now);

        printf("[%5d] %s %s\n", msgnum++, msg, timestr);
    }
}
