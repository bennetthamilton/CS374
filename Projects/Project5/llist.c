// Name: Bennett Hamilton
// Date: 2/9/24
// Description: implementation of linked list 

#include "llist.h"
#include <stdio.h>
#include <stdlib.h>

// insert node at head, previously allocated
void llist_insert_head(struct node **head, struct node *n) {

}

// delete node at head, return pointer to node or NULL if list is empty
struct node *llist_delete_head(struct node **head) {
    return NULL;  
}

// insert node at tail, previously allocated
void llist_insert_tail(struct node **head, struct node *n) {

}

// print list to standard output in the format: "1 -> 2 -> 3" or "[empty]"
void llist_print(struct node *head) {

}

// free entire list, head set to NULL
void llist_free(struct node **head) {

}

// allocate new node with specified value and next pointer set to NULL
struct node *node_alloc(int value) {
    return NULL;
}

// free node
void node_free(struct node *n) {
    free(n);
}

// main function to test linked list implementation
int main(int argc, char *argv[]) {

    // set head pointer
    struct node *head = NULL;

    // parse command line arguments
    for (int i = 1; i < argc; i++) {
        char *cmd = argv[i];
        int value = atoi(argv[i+1]);
        // ref: https://www.programiz.com/c-programming/library-function/string.h/strcmp#google_vignette
        if (strcmp(cmd, "ih") == 0) { // insert node at head
            struct node *n = node_alloc(value);
            llist_insert_head(&head, n);
        } else if (strcmp(cmd, "dh") == 0) { // delete node at head
            struct node *deleted = llist_delete_head(&head);
            if (deleted != NULL) {
                node_free(deleted); // otherwise does nothing
            }
        } else if (strcmp(cmd, "it") == 0) { // insert node at tail
            struct node *n = node_alloc(value);
            llist_insert_tail(&head, n);
        } else if (strcmp(cmd, "p") == 0) { // print list
            llist_print(head);
        } else if (strcmp(cmd, "f") == 0) { // free entire list
            llist_free(&head);
        } else {                            // error check for invalid command
            printf("Invalid command: %s\n", cmd);
        }
    }

    return 0;
}
