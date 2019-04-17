#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Num of args: %d\n", argc);
    for (int i = 0; i < argc; i++)
        printf("Arg[%d]: %s\n", i, argv[i]);

    fprintf(stderr, "this is to stderr\n");
}
