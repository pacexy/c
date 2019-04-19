#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
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
    struct hostent *hp;
    int sockfd;
    char *msg;
    socklen_t sl;

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    
    msg = argv[3];
    sl = sizeof(sa);
    bzero((void *) &sa, sl);
    hp = gethostbyname(argv[1]);
    bcopy((void *) hp->h_addr_list[0], (void *) &sa.sin_addr, hp->h_length);
    sa.sin_port = htons(atoi(argv[2]));
    sa.sin_family = AF_INET;

    printf("%s\n", inet_ntoa(sa.sin_addr));
    //strlen(msg) is necessary, BUFSIZ will cause error
    if (sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *) &sa, sl) == -1)

        oops("sendto");

    return 0;
}
