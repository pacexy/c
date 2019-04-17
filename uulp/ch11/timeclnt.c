#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define oops(x) {perror(x); exit(1);}

int main(int argc, char *argv[])
{
    int sockfd;
    struct hostent *he;
    struct sockaddr_in sockaddr;
    int msglen;
    char message[BUFSIZ];

    /* step1 */
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        oops("socket");

    /* step2 */
    bzero((void *) &sockaddr, sizeof(sockaddr));
    he = gethostbyname(argv[1]);
    bcopy((void *) he->h_addr_list[0], (void *) &sockaddr.sin_addr, he->h_length);
    sockaddr.sin_port = htons(atoi(argv[2]));
    sockaddr.sin_family = AF_INET;
    if (connect(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1)
        oops("connect");
    /* step3 */
    if ((msglen = read(sockfd, message, BUFSIZ)) <= 0)
        oops("read");

    /* step4 */
    write(1, message, msglen);
    close(sockfd);


}
