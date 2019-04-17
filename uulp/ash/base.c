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

    printf("%d\n", i);
    if (i == 0)
        return 0;

    return 1;
}

char **parse(char *cmd)
{
    char **arglist;
    char *arg_pos;
    if ((arglist = (char **) malloc(MAXARG * sizeof(char *))) == NULL)
        return NULL;

    extern int i;
    int j = 0;

    for (j = 0;j < MAXARG-1; j++) {
        while (isspace(cmd[i])) i++;
        if (cmd[i] == '\0')
            return NULL;
        arg_pos = cmd + i;
        while (!isspace(cmd[i]) && cmd[i]!=';') i++;
        printf("ss\n");
        arglist[j] = makestr(arg_pos, &cmd[i]);
        if (cmd[i] == '\n' || cmd[i] == ';') {
            i++;
            j++;
            break;
        }
    }

    arglist[j] = NULL;
    return arglist;
}

void execute(char **arglist)
{
    int pid;
    int stat;

    if ((pid = fork()) == -1)
        oops("fork")
    else if (pid == 0) {
        /* execvp(arglist[0], arglist); */
        /* oops(arglist[0]) */
        /* printf("arglist:%s\n", arglist[0]); */
    }
    else
        while (wait(&stat) != pid) ;
}


char *makestr(char *start, char *end)
{
    static char *str;
    int n = start - end;
    str = (char *) malloc((n+1) * sizeof(char));
    if (str == NULL) {
        printf("str");
        return NULL;
    }
    strncpy(str, start, n);
    str[n] = '\0';
    printf("arglist:%s\n", str);
    return str;
}
