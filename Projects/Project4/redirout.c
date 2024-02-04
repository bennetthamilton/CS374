// Name: Bennett Hamilton
// Date: 2/3/24
// Description: This program will run a child process/program, and save that child's output in a specified file.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    // argument check
    if (argc < 3) {
        perror("Usage: redirout <output file> <command> [args]");
        return 1;
    }

    // open the output file
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    // duplicate the file descriptor to stdout
    dup2(fd, STDOUT_FILENO);

    // close the original file descriptor
    close(fd);

    // run the command
    execvp(argv[2], &argv[2]);
    perror("execvp");

    return 0;

}