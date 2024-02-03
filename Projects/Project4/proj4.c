// Name: Bennett Hamilton
// Date: 2/3/24
// Description: a program that runs ls for a given directory and wc (word count) at the same time, 
//              and pipes the output from ls into wc

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // argument check
    if (argc < 2) {
        perror("Usage: proj4 <directory>");
        exit(EXIT_FAILURE);
    }

    // parse the command line arguments
    const char *directory = (argc > 1) ? argv[1] : ".";
    
    // create a pipe
    int pfd[2];
    pid_t child_pid;

    if (pipe(pfd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // fork a child process
    if ((child_pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {   // child process (run wc)
        // duplicate read end of pipe to stdin
        dup2(pfd[0], STDIN_FILENO);
        // close write end of pipe
        close(pfd[1]);
        // run wc
        execlp("wc", "wc", "-l", NULL);
    
    } else {    // parent process (run ls)
        // duplicate write end of pipe to stdout
        dup2(pfd[1], STDOUT_FILENO);
        // close read end of pipe
        close(pfd[0]);
        // run ls
        execlp("ls", "ls", "-1a", directory, NULL);
    }

    return 0;
}