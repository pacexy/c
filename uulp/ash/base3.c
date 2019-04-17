#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ash3.h"
#define MAXARG 128
#define prompt "$ "

char *makestr(char *, char *);

char *next_cmd()
{
    char *cmd;
    int c;
    int i = 0;
    int bufspace = 0;
    
    printf("%s", prompt);
    while ((c = getc(stdin)) != EOF) {
        if (i+1 >= bufspace) {
            if (bufspace == 0)
                cmd = (char *) malloc(BUFSIZ);
            else 
                cmd = (char *) realloc(cmd, BUFSIZ);

            bufspace += BUFSIZ;
        }
        cmd[i++] = c;
        if (c == '\n')
            break;
    }

    if (i == 0)
        return NULL;

    cmd[i] = '\0';

    /* printf("%d\n", i); */

    if (i > 4 && strncmp(cmd, "exit", 4) == 0 && isspace(cmd[i-1]))
        return NULL;

    return cmd;
}

char **parse(char *cmd)
{
    extern int i;
    int j = 0;
    char **arglist;
    char *arg_pos;
    int spots = 0;
    /* if ((arglist = (char **) malloc(MAXARG * sizeof(char *))) == NULL) */
    /*     return NULL; */
    while (1) {
        if (j >= spots) {
            if (spots == 0)
                arglist = (char **) malloc(BUFSIZ);
            else 
                arglist = (char **) realloc(arglist, BUFSIZ);
            spots += BUFSIZ / sizeof(char *);
        }

        while (isspace(cmd[i])) i++;
        if (cmd[i] == '\0')
            if (j == 0)
                return NULL;
            else
                break;
        else if (cmd[i] == ';') {
            i++;
            break;
        }

        arg_pos = cmd + i;
        while (!isspace(cmd[i]) && cmd[i]!=';') i++;
        arglist[j] = makestr(arg_pos, &cmd[i]);
    /* if (cmd[i] ==  ';') */
        i++;
        j++;
    }

    arglist[j] = NULL;
    return arglist;
}

int execute(char **arglist)
{
    int pid;
    int stat;

    if (arglist[0] == NULL)
        return 0;

    if ((pid = fork()) == -1)
        oops("fork");
    else if (pid == 0) {
        sig_mode(1);
        execvp(arglist[0], arglist);
        oops(arglist[0]);
        /* printf("arglist:%s\n", arglist[0]); */
    }
    else {
        if(wait(&stat) != pid) 
            perror("wait");
        return stat;
    }
}

void freelist(char **arglist)
{
    for(int i = 0; arglist[i]; i++)
        free(arglist[i]);
    free(arglist);
}


char *makestr(char *start, char *end)
{
    static char *str;
    int n = end - start;
    str = (char *) malloc((n+1) * sizeof(char));
    if (str == NULL) {
        return NULL;
    }
    strncpy(str, start, n);
    str[n] = '\0';
    /* printf("arglist:%s\n", str); */
    return str;
}

void sig_mode(int mode)
{
    if (mode) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
    }
    else {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
    }
}

void oops(void *s)
{
    perror((char *) s);
    exit(1);
}
