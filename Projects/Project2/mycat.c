//step #5 write a clone of the cat unix utility

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 2048

int main(int argc, char* argv[]) {

    if (argc == 1) {
        int fd = STDIN_FILENO;

        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;

        // Using while loop to read/write for better memory management
        while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {
            ssize_t bytesWritten = write(STDOUT_FILENO, buffer, bytesRead);

            if (bytesWritten == -1) {
                perror("Error writing to standard output");
                exit(EXIT_FAILURE);
            }
        }

        if (bytesRead == -1) {
            perror("Error reading from standard input");
            exit(EXIT_FAILURE);
        }

        return 0;

    } else {
        for (int i = 1; i < argc; ++i) {
            // open file
            int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                perror("Error opening file");
                exit(EXIT_FAILURE);
            }

            // read file
            char buffer[BUFFER_SIZE];
            ssize_t bytesRead;
            // using while loop to read/write for better memory management
            while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {
                ssize_t bytesWritten = write(STDOUT_FILENO, buffer, bytesRead);

                if (bytesWritten == -1) {
                    perror("Error writing to standard output");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }

            if (bytesRead == -1) {
                perror("Error reading from file");
                close(fd);
                exit(EXIT_FAILURE);
            }

            //close file
            if (close(fd) == -1) {
                perror("Error closing file");
                exit(EXIT_FAILURE);
            }
        

        }

    return 0;
    }

}