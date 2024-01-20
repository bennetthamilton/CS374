// step #3 access command line arguments

#include <stdio.h>

// Write a program called printcl.c that prints out all the command line arguments passed to it like so:

// % ./printcl aaa bb c
// 0: ./printcl
// 1: aaa
// 2: bb
// 3: c
int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i) {
        printf("%d: %s\n", i, argv[i]);
    }

    return 0;
}