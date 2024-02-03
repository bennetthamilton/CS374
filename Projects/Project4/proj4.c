// Name: Bennett Hamilton
// Date: 2/3/24
// Description: a program that runs ls for a given directory and wc (word count) at the same time, 
//              and pipes the output from ls into wc

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    
    // create a pipe

    // fork a child process

    // child process (run wc)
        // duplicate read end of pipe to stdin
        // close write end of pipe
        // run wc

    // parent process (run ls)
        // duplicate write end of pipe to stdout
        // close read end of pipe
        // run ls
        
    return 0;
}