#include <string.h>

enum states {IF_BLK, THEN_BLK, ELSE_BLK, NEUTRAL_BLK};
enum result {SUCC, FAIL};

int state = NEUTRAL_BLK;
int if_stat = SUCC;

int process(char **arglist)
/* return a state value(succ or fail) */
{
    int stat;

    if (iscontrol(arglist[0]))
        stat = move(arglist);
    else if (ok_to_execute())
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

int move(char **args)
{
    char *arg = args[0];
    int stat = FAIL;
    if (strcmp(arg, "if") == 0) {
        if (state == NEUTRAL_BLK) {
            if_stat = process(args+1);
            state = IF_BLK;
            stat = SUCC;
        }
    }
    else if (strcmp(arg, "then") == 0) {
        if (state == IF_BLK) {
            state = THEN_BLK;
            stat = SUCC;
        }
    }
    else if (strcmp(arg, "else") == 0) {
        if (state == THEN_BLK) {
            state = ELSE_BLK;
            stat = SUCC;
        }
    }
    else if (strcmp(arg, "fi") == 0) {
        if (state == THEN_BLK || state == ELSE_BLK) {
            state = NEUTRAL_BLK;
            stat = SUCC;
        }
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
