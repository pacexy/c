#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>

void process_request(char *, int);
int main()
{
    char request[BUFSIZ];
    char host[BUFSIZ];
    int sockfd;
    int fd;
    FILE *fpin;
    FILE *fpout;
    struct sockaddr_in sa;
    struct hostent *hp;

    /* sockfd = socket(PF_INET, SOCK_STREAM, 0); */

    /* gethostname(host, BUFSIZ); */
    /* hp = gethostbyname(host); */
    /* bzero((void *) &sa, sizeof(sa)); */
    /* bcopy((void *) hp->h_addr_list[0], (void *) &sa.sin_addr, hp->h_length); */
    /* sa.sin_family = AF_INET; */
    /* sa.sin_port = 2333; */
    /* if (bind(sockfd, (struct sockaddr *) &sa, sizeof(sa)) == -1) */
    /*     oops("bind"); */

    /* if (listen(sockfd, 1) == -1) */
    /*     oops("listen"); */
    sockfd = make_server_socket(2333);

    while (1) {
        fd = accept(sockfd ,NULL, NULL);
        fpin = fdopen(sockfd, "r");
        fgets(request, BUFSIZ, fpin);
        skip_rest(fpin);
        process_request(request, fd);
        fclose(fpin);
        /* close(fd); */
    }
}
int isnone(char *arg)
{
    
}
void do_not(void)
{
    printf("format: GET HOST HTTP/version\n");
}

void do_404(char *arg)
{
    printf("ERROR: file %s is not found.\n", arg);
}

void do_ls(char *arg, int fd)
{
    dup2(fd, 1);
    close(fd);
    execlp("ls", "ls", arg, NULL);
}

void do_cgi(char *arg, int fd)
{
    dup2(fd, 1);
    close(fd);
    execlp(arg, arg, NULL);
}

void do_cat(char *arg, int fd)
{
    dup2(fd, 1);
    close(fd);
    execlp("cat", arg, NULL);
}
void process_request(char *request, int fd)
{
    char cmd[11], arg[513], ver[11];

    sscanf(request, "%10s %512s %10s", cmd, arg, ver);

    if (fork() != 0)
        return;
    
    if (strcmp(cmd, "GET") != 0)
        do_not(cmd);
    else if (isnone(arg))
        do_404(arg);
    else if (isdir(arg))
        do_ls(arg, fd);
    else if (iscgi(arg))
        do_cgi(arg, fd);
    else 
        do_cat(arg, fd);
}
