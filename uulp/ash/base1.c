#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAXARG 128
#define prompt "$ "
#define oops(x) {perror(x); exit(1);}

char *makestr(char *, char *);
void sig_mode(int);

int next_cmd(char *cmd)
{
    int c;
    int i = 0;
    printf("%s", prompt);
    while ((c = getc(stdin)) != EOF && i < BUFSIZ-1) {
        cmd[i++] = c;
        if (c == '\n')
            break;
    }

    cmd[i] = '\0';

    /* printf("%d\n", i); */
    if (i == 0)
        return 0;

    if (i > 4 && strncmp(cmd, "exit", 4) == 0 && isspace(cmd[i-1]))
        return 0;

    return 1;
}

char **parse(char *cmd)
{
    extern int i;
    int j = 0;
    char **arglist;
    char *arg_pos;
    if ((arglist = (char **) malloc(MAXARG * sizeof(char *))) == NULL)
        return NULL;

    for (j = 0;j < MAXARG-1; j++) {
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
    }

    arglist[j] = NULL;
    return arglist;
}

void execute(char **arglist)
{
    int pid;
    int stat;

    if (arglist[0] == NULL)
        return;

    if ((pid = fork()) == -1)
        oops("fork")
    else if (pid == 0) {
        sig_mode(1);
        execvp(arglist[0], arglist);
        oops(arglist[0])
        /* printf("arglist:%s\n", arglist[0]); */
    }
    else
        while (wait(&stat) != pid) ;
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
