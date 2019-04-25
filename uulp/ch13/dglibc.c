#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define MSGLEN  256 
#define HOSTLEN 256
#define PORT    2333
#define TICKLEN 16

static struct sockaddr_in sa;
static socklen_t sl;
static struct hostent *hp;
static int sockfd;
static int have_ticket;
static int pid;
static char ticket[TICKLEN];

void oops(const char *s)
{
    perror(s);
    exit(1);
}

void narrate(const char *s1, const char *s2)
{
    fprintf(stderr, "CLIENT[%d]: %s %s\n", pid, s1, s2);
}

void setup(void)
{
    char host[HOSTLEN];

    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
        oops("socket");

    pid = getpid();
    sl = sizeof(sa);
    bzero((void *) &sa, sl);
    gethostname(host, HOSTLEN);
    hp = gethostbyname(host);
    bcopy((void *) hp->h_addr_list[0], (void *) &sa.sin_addr, hp->h_length);
    sa.sin_port = htons(PORT);
    sa.sin_family = AF_INET;
}

int get_ticket(void)
{
    if (have_ticket)
        return -1;

    char msg[MSGLEN];
    char *response, *transaction(char *);

    sprintf(msg, "HELO %d", pid);
    if ((response = transaction(msg)) == NULL)
        return -1;

    if (strncmp(response, "TICK", 4) == 0) {
        strcpy(ticket, response + 5);
        narrate("got ticket", ticket);
        have_ticket = 1;
        return 0;
    }
    else if (strncmp(response, "FAIL", 4) == 0)
        narrate("Could not get ticket", response);
    else
        narrate("invalid response", response);
    return -1;
}

int do_regular_work(void)
{
    char msg[MSGLEN];
    char *response, *transaction(char *);
    sprintf(msg, "VALD %s", ticket);

    printf("SuperSleep version 1.0 Running -Licensed Software\n");
    sleep(10);
    if ((response = transaction(msg)) == NULL)
        return -1;

    if (strncmp(response, "GOOD", 4) == 0) {
        narrate("valid ticket", "");
        sleep(10);
        return 0;
    }
    else if (strncmp(response, "FAIL", 4) == 0)
        narrate("invalid ticket", "");
    else
        narrate("invalid response", response);

    return -1;
}

int release_ticket(void)
{
    char msg[MSGLEN];
    char *response, *transaction(char *);
    
    if (!have_ticket)
        return 0;

    sprintf(msg, "GBYE %s", ticket);
    if ((response = transaction(msg)) == NULL)
        return -1;

    if (strncmp(response, "THNX", 4) == 0) {
        narrate("released OK", "");
        return 0;
    }
    else if (strncmp(response, "FAIL", 4) == 0)
        narrate("released FAIL", "");
    else
        narrate("invalid response", response);
    return -1;
}

void finish(void)
{
    close(sockfd);
}

char *transaction(char *msg)
{
    static char response[MSGLEN];

    if (sendto(sockfd, msg, MSGLEN, 0, (struct sockaddr *)&sa, sl) == -1) {
        perror("sendto");
        return NULL;
    }

    if (recvfrom(sockfd, response, MSGLEN, 0, (struct sockaddr *)&sa, &sl) == -1) {
        perror("recvfrom");
        return NULL;
    }

    return response;
}

