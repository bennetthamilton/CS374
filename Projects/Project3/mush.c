#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

void parseInput(char* input, char** args) {

    // Use strtok() to split the input into individual words
    char* token = strtok(input, " \t\n");
    int i = 0;

    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }

    args[i] = NULL; // Null-terminate the array of arguments
}

int main() {
    char input[MAX_INPUT_SIZE];
    char* args[MAX_INPUT_SIZE / 2 + 1]; // Maximum number of arguments

    while (1) {
        // Print the shell prompt
        printf("mush> ");
        fflush(stdout);

        // Read a line of input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        // Remove the newline character from the input
        input[strcspn(input, "\n")] = '\0';

        // Parse the input into individual words
        parseInput(input, args);

        // For now, simply print the parsed arguments
        for (int i = 0; args[i] != NULL; ++i) {
            printf("Prompt %d: %s\n", i, args[i]);
        }

        // TODO: Add the logic for executing commands (fork, execvp, wait)
        // ...

        // TODO: Add error handling and exit conditions
        // ...
    }


    return 0;
}