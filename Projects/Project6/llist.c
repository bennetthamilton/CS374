// Name: Bennett Hamilton
// Date: 2/9/24
// Description: implementation of linked list 

#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// insert node at head, previously allocated
void llist_insert_head(struct node **head, struct node *n) {
    if (n == NULL) {
        return;
    }

    n->next = *head;
    *head = n;
}

// delete node at head, return pointer to node or NULL if list is empty
struct node *llist_delete_head(struct node **head) {
    if (*head == NULL) {
        return NULL;  
    }

    struct node *deleted = *head;   // save pointer to head
    *head = (*head)->next;          // move head to next node
    deleted->next = NULL;           // disconnect deleted node
    return deleted;
}

// insert node at tail, previously allocated
void llist_insert_tail(struct node **head, struct node *n) {
    if (n == NULL) {
        return;
    } else if (*head == NULL) {
        *head = n;
        return;
    } else {
        struct node *current = *head;
        while (current->next != NULL) { // find last node
            current = current->next;
        }
        current->next = n;
    }
}

// print list to standard output in the format: "1 -> 2 -> 3" or "[empty]"
void llist_print(struct node *head) {
    if (head == NULL) {
        printf("[empty]\n");
        return;
    }

    // loop through list and print each value
    while (head != NULL) {
        printf("%d", head->size);
        if (head->next != NULL) {
            printf(" -> ");
        }
        head = head->next;
    }
    printf("\n");
}

// free entire list, head set to NULL
void llist_free(struct node **head) {
    while (*head != NULL) {
        struct node *deleted = llist_delete_head(head);
        node_free(deleted);
    }
}

// TODO: implement in_use flag
// allocate new node with specified value and next pointer set to NULL
struct node *node_alloc(int size, int in_use) {
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if (new_node != NULL) {
        new_node->size = size;
        new_node->next = NULL;
    }
    return new_node;
}

// free node
void node_free(struct node *n) {
    free(n);
}

