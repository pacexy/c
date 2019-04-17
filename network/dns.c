#include <stdio.h>
#include <netdb.h>

int main(int argc, char **argv)
{
    struct hostent *hp;

    hp = gethostbyname(argv[1]);
    printf("%s", hp->h_name);

    return 0;
}
