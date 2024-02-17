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
    // initialize big block of memory using mmap
    void *heap = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);

    // create first block
    memory_block = (struct block *)heap;
    memory_block->size = sizeof(struct block);
    memory_block->in_use = 0;
    memory_block->next = NULL;
}

// function to allocate memory
void *mymalloc(int size) {
    // initialize memory if not already
    if (memory_block == NULL) {
        initialize_memory();
    }
    
    // find first block that is not in use and has enough space

}

void myfree() {
    // TODO: part 2 of project
}

int main() {
    return 0;
}