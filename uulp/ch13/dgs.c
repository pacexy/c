#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //define struct sockaddr_in
#include <unistd.h> //gethostname
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <arpa/inet.h>

void oops(char *s)
{
    perror(s);
    exit(1);
}
int main(int argc, char *argv[])
{
    struct sockaddr_in sa;
    struct sockaddr_in sa_c;
    struct hostent *hp;
    char host[BUFSIZ];
    char msg[BUFSIZ];
    char reply[BUFSIZ];
    char c_ip[BUFSIZ];
    int sockfd;
    int port;
    int c_port;
    int msglen;
    socklen_t sl;

    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
        oops("socket");

    port = atoi(argv[1]);
    sl = sizeof(sa);
    bzero((void *) &sa, sl);
    gethostname(host, BUFSIZ);
    hp = gethostbyname(host);
    //don't forget the [0]
    bcopy((void *) hp->h_addr_list[0], (void *) &sa.sin_addr, hp->h_length);
    sa.sin_port = htons(port);
    sa.sin_family = AF_INET;

    if (bind(sockfd, (struct sockaddr*) &sa, sl) == -1)
        oops("bind");

    while ((msglen = recvfrom(sockfd, msg, BUFSIZ, 0, (struct sockaddr*) &sa_c, &sl)) > 0) {
        /* msg[msglen] = '\0'; */
        printf("get a message: %s\n", msg);
        /* printf("msglen: %d\n", msglen); */
        strncpy(c_ip, inet_ntoa(sa_c.sin_addr), BUFSIZ);
        c_port = ntohs(sa_c.sin_port);
        /* printf("server ip: %s:%d\n", inet_ntoa(sa.sin_addr), port); */
        /* printf("custom ip: %s:%d\n", c_ip, c_port); */
        /* if /n is ignore, fflush should be added to flush the buffer */
        /* fflush(stdout); */
        sprintf(reply, "have received your msg\n");
        if (sendto(sockfd, reply, strlen(reply), 0, (struct sockaddr *) &sa_c, sl) == -1)
            oops("sendto");
    }

    return 0;
}
