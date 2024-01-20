//step #6 write a utility that turns everything to UPPERCASE

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define BUFFER_SIZE 2048

void read_and_write(int fileDescripter) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    
    while ((bytesRead = read(fileDescripter, buffer, BUFFER_SIZE)) > 0) {
        // Convert each character to uppercase using toupper()
        for (ssize_t i = 0; i < bytesRead; ++i) {
            buffer[i] = toupper(buffer[i]);
        }

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
}

int main(int argc, char* argv[]) {

    if (argc == 1) {
        int fd = STDIN_FILENO;

        read_and_write(fd);

        return 0;

    } else {
        for (int i = 1; i < argc; ++i) {
            // open file
            int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                perror("Error opening file");
                exit(EXIT_FAILURE);
            }

            read_and_write(fd);

            //close file
            close(fd);

        }

    return 0;
    }

}