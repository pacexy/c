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
    struct sockaddr_in sa_s;
    struct hostent *hp;
    int sockfd;
    char *msg;
    char reply[BUFSIZ];
    socklen_t sl;

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    
    msg = argv[3];
    sl = sizeof(sa);
    bzero((void *) &sa, sl);
    hp = gethostbyname(argv[1]);
    bcopy((void *) hp->h_addr_list[0], (void *) &sa.sin_addr, hp->h_length);
    sa.sin_port = htons(atoi(argv[2]));
    sa.sin_family = AF_INET;

    //strlen(msg) is necessary, BUFSIZ will cause error
    if (sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *) &sa, sl) == -1)
        oops("sendto");
    
    if (recvfrom(sockfd, reply, BUFSIZ, 0, NULL, NULL) == -1)
        oops("recvfrom");

    printf("%s\n", reply);
    return 0;
}
