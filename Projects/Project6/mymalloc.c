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

void *find_space_and_split(struct block *head, int new_size) {
    struct block *current = head;

    while (current != NULL) {
        // Look for blocks not in use
        if (!current->in_use) {

            if (current->size >= new_size) {

                if (current->size == new_size) {
                    current->in_use = 1;
                    return PTR_OFFSET(current, PADDED_SIZE(sizeof(struct block)));
                }

                int size_diff = current->size - new_size;
                
                int new_current_size = current->size - size_diff;

                current->size = new_current_size;
                current->in_use = 1;

                struct block *new_block = PTR_OFFSET(current, PADDED_SIZE(sizeof(struct block)) + new_current_size);
                new_block->size = size_diff - PADDED_SIZE(sizeof(struct block));
                new_block->in_use = 0;

                current->size = new_size;

                new_block->next = current->next;
                current->next = new_block;

                return PTR_OFFSET(current, PADDED_SIZE(sizeof(struct block))); 
            }   
            
        }

        current = current->next;
    }

    return NULL;
}

void coalesce_space(struct block *head)
{
    // check for empty list or single node
    if (head == NULL || head->next == NULL)               
        return;

    // set current node to head
    struct block *current = head;    
    
    // scan the list via loop
    while (current->next != NULL) { 
        // check for sequential nodes that are not in use
        if (current->in_use == 0 && current->next->in_use == 0) {
            struct block *temp = current->next;     // remove the next node(s)
            current->next = temp->next;
            current->size += temp->size;            // add their sizes to the first node
            free(temp);                             // free the removed node(s)
        } else {    // move to the next node
            current = current->next;
        }
    }
}

void *myalloc(int size) {
    // initialize memory if not already
    if (head == NULL) {
        initialize_memory();
    }

    // get padded size
    int padded_size = PADDED_SIZE(size);

    // find first block that is not in use and has enough space
    void *allocated_block = find_space_and_split(head, padded_size);

    // coalesce free space after allocation
    coalesce_space(head);

    // no block found
    return allocated_block;
}

void myfree(void *ptr) {
    // subtract the offset to get the pointer to the corresponding block structure
    struct block *block_to_free = (struct block *)((char *)ptr - PADDED_SIZE(sizeof(struct block)));

    // mark the block as not in use
    block_to_free->in_use = 0;
}

void print_data(void){
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // // Uncomment the following line if you want to see the pointer values
        // printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
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

    p = myalloc(10); print_data();

    myfree(p); print_data();

}