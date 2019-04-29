#include <sys/types.h>
#include <unistd.h>

int main()
{
    if (fork() == 0)
        sleep(10);
    else
        sleep(5);
    return 0;
}
