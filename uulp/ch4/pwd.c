#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>
#include <string.h>

ino_t get_inode(const char *);
const char *get_dirname(ino_t, char *, int);
void print_path(ino_t);

int main(void)
{
    print_path(get_inode("."));
    putchar('\n');

    return 0;
}

void print_path(ino_t this_inode)
{
    char name[BUFSIZ];
    if (this_inode != get_inode("..")) {
        chdir("..");
        get_dirname(this_inode, name ,BUFSIZ);
        print_path(get_inode("."));
        printf("/%s", name);
        /* printf("/%s", get_dirname(this_inode, name, BUFSIZ)); */
    }
}

ino_t get_inode(const char *fname)
{
    struct stat st;
    stat(fname, &st);
    return st.st_ino;
}

const char *get_dirname(ino_t inode, char *name, int len)
{
    DIR *dirp;
    struct dirent *direntp;
    dirp = opendir(".");
    do {
        direntp = readdir(dirp);
    } while (direntp->d_ino != inode);

    strncpy(name, direntp->d_name, len);
    name[len-1] = '\0';
    closedir(dirp);

    return name;
}
