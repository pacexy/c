#include <stdio.h>

int main() 
{
    char buf[100];
    FILE *fp;
    int i = 0;

    if ((fp = popen("who|sort", "r")) == NULL)
        perror("popen");


    while (fgets(buf, 100, fp) != NULL)
        printf("%d %s\n", i++, buf);
}
