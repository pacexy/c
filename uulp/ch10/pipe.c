#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define CHILD_MES "I want a cookie...\n"
#define PAR_MES   "cookie...\n"
int main()
{
    int pfd[2];
    int len;
    char buf[BUFSIZ];
    int read_len;

    if (pipe(pfd) == -1)
        perror("pipe");

    switch (fork()) {
        case -1:
            perror("fork");
            break;
        case 0:
            len = strlen(CHILD_MES);
            while (1) {
                if (write(pfd[1], CHILD_MES, len) != len)
                    perror("write");
                sleep(3);
            }
            break;
        default:
            len = strlen(PAR_MES);
            while (1) {
                if (write(pfd[1], PAR_MES, len) != len)
                    perror("write");
                sleep(1);
                read_len = read(pfd[0], buf, BUFSIZ);
                if (read_len <= 0)
                    break;
                write(1, buf ,read_len);
            }
    }
}
