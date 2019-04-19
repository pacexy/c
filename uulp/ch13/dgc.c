#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in sa;
    struct hostent *hp;
    int sockfd;
    socklen_t sl;

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    
    sl = sizeof(sa);
    bzero((void *) &sa, sl);
    hp = gethostbyname(argv[1]);
    bcopy((void *) hp->h_addr_list[0], (void *) &sa.sin_addr, hp->h_length);
    sa.sin_port = htons(atoi(argv[2]));
    sa.sin_family = AF_INET;

    sendto(sockfd, argv[3], BUFSIZ, 0, (struct sockaddr *) &sa, sl);

    return 0;
}
