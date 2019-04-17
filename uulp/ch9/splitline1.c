#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "smsh.h"

char *next_cmd(char *prompt, FILE *fp)
{
    char *buf;
    int bufspace = 0;
    int pos = 0;
    int c;

    printf("%s", prompt);
    while ((c = getc(fp)) != EOF) {
        if (pos+1 >= bufspace) {
            if (bufspace == 0)
                buf = (char *) emalloc(BUFSIZ);
            else
                buf = (char *) erealloc(buf, bufspace + BUFSIZ);
            bufspace += BUFSIZ;
        }

        if (c == '\n')
            break;

        buf[pos++] = c;
    }
    if (c == EOF && bufspace == 0)
        return NULL;
    buf[pos] = '\0';
    return buf;
}

char **splitline(char *line)
{
    char *newstr(char *, int);
    char **args;
    int is_delim(char);
    int spots = 0; 
    int bufspace = 0;
    int argnum = 0;
    char *cp = line;
    char *start;
    int len;

    /* if (line == NULL) */
        /* return NULL; */

    args = (char **) emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    spots = BUFSIZ/sizeof(char *);
    
    while (*cp != '\0') {
        while (is_delim(*cp))
            cp++;
        if (*cp == '\0')
            break;

        if (argnum+1 >= spots) {
            args = (char **) erealloc(args, bufspace+BUFSIZ);
            bufspace += BUFSIZ;
            spots += (BUFSIZ/sizeof(char *));
        }
        start = cp;
        len = 1;
        while (*++cp != '\0' && !(is_delim(*cp)))
            len++;
        args[argnum++] = newstr(start, len);
    }
    args[argnum] = NULL;
    return args;
}

char *newstr(char *s, int l)
{
    char *rv = (char *) emalloc(l+1);
    strncpy(rv, s, l);
    rv[l] = '\0';
    return rv;
}

void freelist(char **list)
{
    char **cp = list;
    while (*cp != NULL)
        free(*cp++);
    free(list);
}

void *emalloc(size_t n)
{
    void *rv;
    if ((rv = malloc(n)) == NULL)
        fatal((char *) "out of memory", (char *) "", 1);
    return rv;
}

void *erealloc(void *p, size_t n)
{
    void *rv;
    if ((rv = (char *) realloc(p, n)) == NULL)
        fatal((char *) "out of memory", (char *) "", 1);
    return rv;
}

int is_delim(char c)
{
    if (isspace(c) || c == '\0')
        return 0;
    return 1;
}
