// Name: Bennett Hamilton
// Date: 2/17/24
// Description: my implementation of memory allocator

#include "mymalloc.h"
#include <stddef.h>
#include <sys/mman.h>

// function to construct memory block
struct block {
    int size;
    int in_use;
    struct block *next;
};

static struct block *memory_block = NULL;

// function to initialize memory
void initialize_memory() {
    // TODO
}

// function to allocate memory
void *my_malloc(int size) {
    // initialize memory if not already
    if (memory_block == NULL) {
        initialize_memory();
    }
}

int main() {
    return 0;
}