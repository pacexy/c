#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>

void oops(char *s);
int make_server_socket_q(int, int);

int make_server_socket(int port)
{
    return make_server_socket_q(port, 1);
}

int make_server_socket_q(int port, int queue)
{
    int sockfd;
    char host[BUFSIZ];
    struct sockaddr_in saddr;
    struct hostent *he;

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        oops("socket");

    bzero((void *) &saddr, sizeof(saddr));
    if (gethostname(host, BUFSIZ) == -1)
        oops("gethostname");
    he = gethostbyname(host);
    bcopy((void *) he->h_addr_list[0], (void *) &saddr.sin_addr, he->h_length);
    saddr.sin_port = htons(port);
    saddr.sin_family = AF_INET;
    if (bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) != 0)
        oops("bind");

    if (listen(sockfd, queue) == -1)
        oops("listen");

    return sockfd;
}

int connect_to_server(char *host, int port)
{
    int sockfd;
    struct sockaddr_in servaddr;
    struct hostent *he;

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        oops("socket");
    
    he = gethostbyname(host);
    bzero((void *) &servaddr, sizeof(servaddr));
    bcopy((void *) he->h_addr_list[0], (void *) &servaddr.sin_addr, he->h_length);
    servaddr.sin_port = htons(port);
    servaddr.sin_family = AF_INET;
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1)
        oops("connect");

    return sockfd;
}

void oops(char *s)
{
    perror(s);
    exit(1);
}
