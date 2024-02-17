// Name: Bennett Hamilton
// Date: 2/17/24
// Description: my implementation of memory allocator

#include "mymalloc.h"
#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

// alignment and pointer macros
#define ALIGNMENT 16   // Must be power of 2
#define HEAP_SIZE 1024  // 1KB
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

// function to construct memory block
struct block {
    int size;
    int in_use;
    struct block *next;
};

static struct block *head = NULL;

// function to initialize memory
void initialize_memory() {
    // initialize big block of memory using mmap
    void *heap = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);

    // error checking
    if (heap == MAP_FAILED) {
        // Handle the error, print an error message or exit the program
        perror("Error initializing memory");
        exit(EXIT_FAILURE);
    }

    // get size of data
    int data_size = HEAP_SIZE - PADDED_SIZE(sizeof(struct block));
    
    // create first block
    head = (struct block *)heap;
    head->size = data_size;
    head->in_use = 0;
    head->next = NULL;
}

// function to allocate memory
void *mymalloc(int size) {
    // initialize memory if not already
    if (head == NULL) {
        initialize_memory();
    }

    // get padded size
    int padded_size = PADDED_SIZE(size);

    // start at beginning of memory block
    struct block *current = head;

    // find first block that is not in use and has enough space
    while (current) {
        // check if the block is not in use and has enough space
        if (!current->in_use && current->size >= padded_size) {
            // TODO: (later) if the block has more space than needed, split it

            current->in_use = 1;            // mark the block as in use
            return (void *)(current + 1);   // return pointer to user data
        }

        current = current->next;
    }

    // no block found
    return NULL;

}

// void myfree() {
//     // TODO: part 2 of project
// }

void print_data(void){
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

// main function for testing
int main() {
    void *p;

    print_data();
    p = mymalloc(64);
    print_data();
}