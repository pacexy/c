#/bin/zsh
cc twebs.c socklib.c -lpthread
./a.out 23333&
telnet pc 23333
