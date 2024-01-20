// step #4 read a file

// Make a program readfile.c that reads the file specified on the command line.
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
    if (argc != 2) {
        perror("Usage: ./readfile <filename>");
        exit(EXIT_FAILURE);
    }

    // open file
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // read file
    char buffer[2048];
    ssize_t bytesRead = read(fd, buffer, 2048);
    if (bytesRead == -1) {
        perror("Error reading from file");
        close(fd);
        exit(EXIT_FAILURE);
    }


    // write output
    ssize_t bytesWritten = write(STDOUT_FILENO, buffer, bytesRead);
    if (bytesWritten == -1) {
        perror("Error writing to standard output");
        close(fd);
        exit(EXIT_FAILURE);
    }

    //close file
    if (close(fd) == -1) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}