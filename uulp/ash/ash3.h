#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char *next_cmd();
char **parse(char *);
int execute(char **);
void freelist(char **);
void sig_mode(int);
void oops(char *);

int iscontrol(char *);
int move(char *);
int ok_to_execute(void);
int syn_err(char *);
