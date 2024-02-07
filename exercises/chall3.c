#include <stdio.h>
#include <stdlib.h>

// Make a deep copy of a ragged array of strings
char **alloc_copy(char **source, int count)
{
    char **copy = malloc(count * sizeof(char *));

    for (int i = 0; i < count; i++) {
        copy[i] = malloc(strlen(source[i]) + 1);
        strcpy(copy[i], source[i]);
    }

    return copy;
}

void free_copy(char **copy, int count)
{
    free(copy - count); 
}

void print_ragged_array(char **array, int count)
{
    for (int i = 0; i < count; i++) {
        printf("\t%s\n", array[i]);
    }
}

void hline()
{
    puts("----------------------------------------------");
}

int main(void)
{
    char *source[] = {
        "Hello!",
        "This an array with elements of different lengths",
        "And we wish to copy it."
    };

    printf("Original array:\n");
    print_ragged_array(source, 3);
    hline();

    char **copy = alloc_copy(source, 3);

    printf("Copy of array:\n");
    print_ragged_array(copy, 3);
    hline();

    // If you only make a shallow copy, this line will crash:
    copy[0][0] = 'x';

    free_copy(copy, 3);
}