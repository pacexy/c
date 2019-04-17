#include <stdio.h>
#include <string.h>
#include "ash3.h"

enum states {IF_BLK, THEN_BLK, ELSE_BLK, NEUTRAL_BLK};
enum result {SUCC, FAIL};

int state = NEUTRAL_BLK;
int if_stat = SUCC;

int process(char **arglist)
/* return a state value(succ or fail) */
{
    int stat = SUCC;

    if (iscontrol(arglist[0]))
        stat = move(arglist);
    if (stat == SUCC && ok_to_execute())
        stat = execute(arglist);

    return stat;
}

int iscontrol(char *arg)
{
    return (strcmp(arg, "if")   == 0 ||
            strcmp(arg, "then") == 0 ||
            strcmp(arg, "else") == 0 ||
            strcmp(arg, "fi")   == 0);
}

int move(char *arg)
{
    int stat = FAIL;
    if (strcmp(arg, "if") == 0) {
        if (state == NEUTRAL_BLK) {
            state = IF_BLK;
            stat = SUCC;
        }
        else syn_err("if");
    }
    else if (strcmp(arg, "then") == 0) {
        if (state == IF_BLK) {
            state = THEN_BLK;
            stat = SUCC;
        }
        else syn_err("then");
    }
    else if (strcmp(arg, "else") == 0) {
        if (state == THEN_BLK) {
            state = ELSE_BLK;
            stat = SUCC;
        }
        else syn_err("else");
    }
    else if (strcmp(arg, "fi") == 0) {
        if (state == THEN_BLK || state == ELSE_BLK) {
            state = NEUTRAL_BLK;
            stat = SUCC;
        }
        else syn_err("fi");
    }

    return stat;
}

int ok_to_execute()
{
    int stat = SUCC;

    if (state == THEN_BLK && if_stat == FAIL)
        stat = FAIL;
    if (state == ELSE_BLK && if_stat == SUCC)
        stat = FAIL;

    return stat;
}

int execute(char **args)
{
    int stat;
    if (iscontrol(args[0])) {
        if (args[1] == NULL)
            return SUCC;
        else if (strcmp(args[0], "fi") != 0)
            stat = process(args+1);
        else
            stat = syn_err("fi");
    }

    int pid = fork();
    if (pid == -1)
        oops("fork");
    else if (pid == 0) {
        sig_mode(1);
        execvp(args[0], args);
        oops(arglist[0]);
    }
    else {
        if (wait(&stat) != pid)
            perror("wait");
        if (state == IF_BLK)
            if_stat = stat>>8 == 0 ? SUCC:FAIL;
        return if_stat;
    }


}
int syn_err(char *s)
{
    state = NEUTRAL_BLK;
    fprintf(stderr, "syntax error: %s\n", s);
    return FAIL;
}

