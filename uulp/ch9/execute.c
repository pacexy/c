#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
int execute(char *argv[])
{
    int pid;
    int child_info = -1;

    if (argv[0] == NULL)
        return 0;

    if ((pid = fork()) == -1) {
        perror("fork");
        exit(2);
    }
    else if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execvp(argv[0], argv);
        perror("execute");
        exit(2);
    }
    else {
        if (wait(&child_info) == -1)
            perror("wait");
    }
    return child_info;
}
