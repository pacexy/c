#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

#include <pthread.h>
#include <dirent.h>
#include <unistd.h>

#include <time.h>

#define MSGLEN 512
#define CMDLEN 8
#define ARGLEN 256

time_t server_started;
int server_request;
int server_sent;

void oops(const char *);
int make_server_socket(int);
void setup(pthread_attr_t *);
void skip_rest(FILE *);
void handle_request(const char *, int);

void qwq_warning(int);
int isBuiltin(const char *);
void qwq_builtin(int);
int notfile(const char *);
void qwq_notfile(int);
int isdir(const char *);
void qwq_ls(const char *, int);
int iscgi(const char *);
void qwq_cgi(const char *, int);
void qwq_cat(const char *, int);

int main(int ac, char *av[])
{
    int sockfd;
    int fd;
    pthread_t t;
    pthread_attr_t attr;
    void *handle_call(void *);

    sockfd = make_server_socket(atoi(av[1]));
    setup(&attr);

    while (1) {
        if ((fd = accept(sockfd, NULL, NULL)) == -1) {
            perror("accept");
            continue;
        }
        server_request++;
        pthread_create(&t, &attr, handle_call, (void *) &fd);

        close(fd);
    }

}

void setup(pthread_attr_t *attrp)
{
    pthread_attr_init(attrp);
    pthread_attr_setdetachstate(attrp, PTHREAD_CREATE_DETACHED);

    server_started = time(NULL);

    /* server_request = 0; */
    /* server_sent = 0; */
}

void *handle_call(void *fdp)
{
    char request[MSGLEN];
    int fd = *(int *) fdp;
    FILE *fp = fdopen(fd, "r");
    
    if (fgets(request, MSGLEN, fp) == NULL) {
        perror("fgets");
        return NULL;
    }

    printf("got a connect on %d: %s\n", fd, request);
    skip_rest(fp);

    handle_request(request, fd);

    fclose(fp);
    return NULL;
}

void skip_rest(FILE *fp)
{
    char msg[MSGLEN];
    while (fgets(msg, MSGLEN, fp) != NULL && strcmp(msg, "\r\n")) ;
}

void handle_request(const char *request, int fd)
{
    char cmd[CMDLEN];
    char arg[ARGLEN];

    if (sscanf(request, "%s %s", cmd, arg) != 2)
        return;

    if (strncmp(request, "GET", 3) != 0)
        qwq_warning(fd);
    else if (isBuiltin(arg))
        qwq_builtin(fd);
    else if (notfile(arg))
        qwq_notfile(fd);
    else if (isdir(arg))
        qwq_ls(arg, fd);
    else if (iscgi(arg))
        qwq_cgi(arg, fd);
    else
        qwq_cat(arg, fd);
}

int http_reply(int fd, FILE **fpp, int code, const char *msg, const char *type, const char *content)
{
    FILE *fp = fdopen(fd, "w");
    int bytes = 0;

    bytes += fprintf(fp, "HTTP/1.0 %d %s\r\n", code, msg);
    bytes += fprintf(fp, "content-type: %s\r\n\r\n", type);
    if (content)
        bytes += fprintf(fp, "%s\r\n", content);
    fflush(fp);

    if (fpp)
        *fpp = fp;
    else
        fclose(fp);

    return bytes;

}
void qwq_warning(int fd)
{
    server_sent += http_reply(fd, NULL, 501, "Invalid request", "text/plain", "Usage: GET [filename]");
}

int isBuiltin(const char *arg)
{
    return  (strcmp(arg, "status") != 0) ? 0:1;
}

void qwq_builtin(int fd)
{
    FILE *fp;
    int bytes = 0;

    bytes += http_reply(fd, &fp, 200, "OK", "text/plain", NULL);
    bytes += fprintf(fp, "Running time: %s\n", ctime(&server_started));
    bytes += fprintf(fp, "Received request: %d\n", server_request);

    server_request += bytes;
    fclose(fp);
}

int notfile(const char *arg)
{
    struct stat st;
    return (stat(arg, &st) == -1) ? 0:1;
}

void qwq_notfile(int fd)
{
    server_sent += http_reply(fd, NULL, 404, "Not found", "text/plain", "No such file");
}

int isdir(const char *arg)
{
    struct stat st;
    stat(arg, &st);
    return S_ISDIR(st.st_mode);
}

void qwq_ls(const char *arg, int fd)
{
    DIR *dirp;
    struct dirent *direntp;
    FILE *fp = fdopen(fd, "w");
    int bytes = 0;

    if ((dirp = opendir(arg)) == NULL)
        return;

    bytes += http_reply(fd, &fp, 200, "OK", "text/plain", NULL);
    bytes += fprintf(fp, "dir %s\n", arg);
    while(direntp = readdir(dirp)) {
        bytes += fprintf(fp, "%s\n", direntp->d_name);
    }

    server_sent += bytes;
    closedir(dirp);
    fclose(fp);
}

char *file_type(const char *arg)
{
    char *extention;
    if ((extention = strchr(arg, '.')) != NULL)
        return extention+1;
    return NULL;
}

int iscgi(const char *arg)
{
    if (strcmp(file_type(arg), "cgi") == 0)
        return 1;
    return 0;
}

void qwq_cgi(const char *arg, int fd)
{
    FILE *fp = fdopen(fd, "w");
    int bytes = 0;

    bytes += http_reply(fd, &fp, 200, "OK", "text/plain", NULL);
    bytes += fprintf(fp, "dir %s\n", arg);

    /* execute cgi */

    server_sent += bytes;
}

void qwq_cat(const char *arg, int fd)
{
    char *extention = file_type(arg);
    char *type = (char *) "text/plain";
    FILE *fpsock = fdopen(fd, "w");
    FILE *fpfile = fopen(arg, "r");
    int c;
    int bytes = 0;

    if (strcmp(extention, "html") == 0)
        type = (char *) "text/html";
    else if (strcmp(extention, "jpg") == 0 || strcmp(extention, "jpeg") == 0)
        type = (char *) "image/jpeg";
    else if (strcmp(extention, "gif") == 0)
        type = (char *) "image/gif";
    else if (strcmp(extention, "png") == 0)
        type = (char *) "image/png";

    while ((c = getc(fpfile)) != EOF) {
        bytes += http_reply(fd, &fpsock, 200, "OK", type, NULL);
        putc(c, fpsock);
        bytes++;
    }
    
    server_sent += bytes;
    fclose(fpsock);
    fclose(fpfile);
}
