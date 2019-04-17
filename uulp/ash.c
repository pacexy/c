#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#define MAXLEN 1024
#define MAXARG 10

char *makearg(void);
void execute(char *[]);

char *command;
int main()
{
    char prompt[] = "$ ";
    char *arglist[MAXARG+1];
    char *comm_start;
    int argnum;
    
    comm_start = (char *) malloc(MAXLEN);
    while (1)
    {
        write(0, prompt, strlen(prompt));
        command = comm_start;

        if (fgets(command, MAXLEN, stdin)) {

            /* printf("%ld\n", strlen(command)); */
            for (argnum = 0; argnum < MAXARG; argnum++)
                if ((arglist[argnum] = makearg()) == NULL)
                    break;

            printf("%d\n", argnum);
            if (argnum == 0)
                continue;
            /* if (feof(stdin)) */
                /* break; */

            if (strcmp(*arglist, "exit") == 0)
                break;
            execute(arglist);
        }
        else
            break;
    }

    free(comm_start);
}

char *makearg()
{
    int i = 0;
    char *ret;


    while (isspace(command[i])) i++;

    if (command[i] == '\0')
        return NULL;

    ret = command + i;
    while (!isspace(command[i])) i++;
    command[i] = '\0';
    command += i+1;

    return ret;
}

void execute(char *arglist[])
{
    int pid;
    int stat;

    if ((pid = fork()) == -1)
        perror("fork");
    else if (pid == 0) {
        execvp(arglist[0], arglist);
        perror(arglist[0]);
        exit(1);
    }
    else {
        while (wait(&stat) != pid) ;
        /* printf("child exited with status %d %d\n", stat>>8, stat & 0x7f); */
    }
}

