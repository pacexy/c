#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define HOSTLEN 256
#define MSGLEN  16
#define MAXNUM  3
#define PORT    2333

static int sockfd;
static int ticket_list[MAXNUM];
static int rest;
static struct sockaddr_in sa;
static socklen_t sl;
static struct hostent *hp;
static char host[HOSTLEN];


char *hello(char *);
char *goodbye(char *);

void oops(const char *s)
{
    perror(s);
    exit(1);
}

void narrate(const char *s1, const char *s2, struct sockaddr_in *sa_c)
{
    char *ip; 
    int port;
    fprintf(stderr, "SERVER: %s:%s ", s1, s2);
    /* if (sa_c) { */
        ip = inet_ntoa(sa_c->sin_addr);
        port = ntohs(sa_c->sin_port);
        fprintf(stderr, "(%s:%d)", ip, port);
    /* } */
    /* fflush(stdout); */
    putc('\n', stderr);
}
void free_ticket(void)
{
    int i;
    for (i = 0; i < MAXNUM; i++)
        ticket_list[i] = 0;
    rest = MAXNUM;
}

void setup(void) 
{
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0))  == -1)
        oops("socket");

    sl = sizeof(sa);
    bzero((void *) &sa, sl);
    gethostname(host, HOSTLEN);
    hp = gethostbyname(host);
    bcopy((void *) hp->h_addr_list[0], (void *) &sa.sin_addr, hp->h_length);
    sa.sin_port = htons(PORT);
    sa.sin_family = AF_INET;

    if (bind(sockfd, (struct sockaddr *) &sa, sl) == -1)
        oops("bind");

    free_ticket();
}

void finish(void)
{
    close(sockfd);
}

void transaction(void)
{
    int msglen;
    char msg[MSGLEN];
    char *response;
    struct sockaddr_in sa_c;

    if ((msglen = recvfrom(sockfd, msg, MSGLEN, 0, (struct sockaddr *) &sa_c, &sl)) == -1)
        perror("recvfrom");

    narrate(" GOT", msg, &sa_c);
    if (strncmp(msg, "HELO", 4) == 0) 
        response = hello(msg);
    else if (strncmp(msg, "GBYE", 4) == 0)
        response = goodbye(msg);
    else 
        response = (char *) "FAIL invalid request";

    if (sendto(sockfd, response, MSGLEN, 0, (struct sockaddr *) &sa_c, sl) == -1)
        perror("sendto");
    narrate("SAID", response, &sa_c);
}

char *hello(char *msg) {
    static char response[MSGLEN];
    int i;

    if (rest <= 0) 
        return (char *) "FAIL no tickets available";

    for (i = 0; i < MAXNUM && ticket_list[i] != 0; i++) ;

    if (i == MAXNUM)
        return (char *) "FAIL database corrupt";

    ticket_list[i] = atoi(msg+5);
    sprintf(response, "TICK %d.%d", ticket_list[i], i);
    rest--;

    return response;
}

char *goodbye(char *msg) {
    int pid;
    int i;
    static char response[MSGLEN];

    if (rest >= 3)
        return (char *) "FAIL too many tickets";

    pid = atoi(msg+5);

    if (sscanf(msg+5, "%d.%d", &pid, &i) != 2 || ticket_list[i] != pid)
        return (char *) "FAIL invalid ticket";
    
    ticket_list[i] = 0;
    sprintf(response, "THNX");
    rest++;

    return response;
}
