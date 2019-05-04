#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void oops(const char *s)
{
    perror(s);
    exit(1);
}
int main(int ac, char *av[])
{
    FILE *fp1;
    FILE *fp2;
    char buf[BUFSIZ];
    if ((fp1 = fopen(av[1], "r")) == NULL)
        oops(av[1]);

    if ((fp2 = fopen(av[2], "w")) == NULL)
        oops(av[2]);

    while (fgets(buf, BUFSIZ, fp1) != NULL) {
        printf("%d\n", (int) strlen(buf));
        fputs(buf, fp2);
    }

    fclose(fp1);
    fclose(fp2);
}
