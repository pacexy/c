#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 2333
#define HOSTLEN 256
#define oops(x) {perror(x); exit(1);}
int main()
{
    int sockfd;
    int fd;
    FILE *fp;
    struct sockaddr_in saddr;
    char hostname[HOSTLEN];
    struct hostent *he;
    time_t t;
    /* step1 */
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        oops("socket");

    /* step2 */
    bzero((void *) &saddr, sizeof(saddr));
    gethostname(hostname, HOSTLEN);
    he = gethostbyname(hostname);
    bcopy((void *) he->h_addr_list[0], (void *) &saddr.sin_addr, he->h_length);
    saddr.sin_port = htons(PORT);
    saddr.sin_family = AF_INET;
    if (bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) != 0)
        oops("bind");

    /* step3 */
    if (listen(sockfd, 1) != 0)
        oops("listen");

    while (1) {
        /* step4 */
        if ((fd = accept(sockfd, NULL, NULL)) == -1)
            oops("accept");

        /* step5 */
        if ((fp = fdopen(fd, "w")) == NULL)
            perror("fdopen");

        t = time(NULL);
        fprintf(fp, "time is %s", ctime(&t));

        /* step6 */
        fclose(fp);
        close(fd);
    }
}
