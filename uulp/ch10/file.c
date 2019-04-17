#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
int main()
{
    printf("%d", stdin->_fileno);
    /* freopen("a", "r", stdin); */
    fclose(stdin);
    stdin = fopen("a", "r");
    printf("%d", stdin->_fileno);
    /* getchar(); */
    printf("%d", open("a", O_RDONLY));
    printf("%d", open("a", O_RDONLY));
    printf("%d", open("a", O_RDONLY));

}
