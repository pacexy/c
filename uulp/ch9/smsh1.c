#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "smsh.h"

#define PROMPT "> "
int main()
{
    char *cmd, *prompt, **arglist;
    int result;
    void setup();

    prompt = PROMPT;
    setup();

    while ((cmd = next_cmd((char *) prompt, stdin) != NULL)) {
        if ((arglist = splitline(cmd)) != NULL) {
            result = execute(arglist);
            freelist(arglist);
        }
        free(cmd);
    }
    return 0;
}

void setup()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}
