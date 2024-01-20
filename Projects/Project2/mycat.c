//step #5 write a clone of the cat unix utility

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 2048

void read_and_write(int fileDescripter) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
     // Using while loop to read/write for better memory management
    while ((bytesRead = read(fileDescripter, buffer, BUFFER_SIZE)) > 0) {
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