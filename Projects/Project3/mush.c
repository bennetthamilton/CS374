#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 2048
#define MAX_ARGUMENTS  128

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

void runCommand (char** args) {
    // Fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process: execute the command
        execvp(args[0], args);
        // If execvp returns, an error occurred
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process: wait for the child to finish
        wait(NULL);
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    char* args[MAX_ARGUMENTS]; 

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

        // if command is cd, call system call chdir
        if (strcmp(args[0], "cd") == 0) {
            if (chdir(args[1]) != 0) {
                perror("chdir");
            }
            continue;
        }

        // if command is exit, exit the program
        if (strcmp(args[0], "exit") == 0) {
            exit(EXIT_SUCCESS);
        }

        if (args[0] == NULL) {
            continue; // Empty command, go to the next iteration
        }

       runCommand(args);

    }

    return 0;
}