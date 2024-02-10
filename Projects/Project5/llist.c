// Name: Bennett Hamilton
// Date: 2/9/24
// Description: implementation of linked list 

#include "llist.h"
#include <stdio.h>
#include <stdlib.h>

void llist_insert_head(struct node **head, struct node *n) {

}

struct node *llist_delete_head(struct node **head) {
    return NULL;  
}

void llist_insert_tail(struct node **head, struct node *n) {

}

void llist_print(struct node *head) {

}

void llist_free(struct node **head) {

}

struct node *node_alloc(int value) {
    return NULL;
}

void node_free(struct node *n) {
    free(n);
}


int main(int argc, char *argv[]) {

    // set head pointer
    struct node *head = NULL;

    // parse command line arguments
    for (int i = 1; i < argc; i++) {
        // insert node at head
        // insert node at tail
        // delete node at head (nothing if empty)
        // free entire list
        // print list
    }

    return 0;
}
