#include <stdio.h>

int main()
{
    FILE *fp;
    fp = popen("mail pacexy backup", "w");
    fprintf(fp, "error\n");
    pclose(fp);
}
