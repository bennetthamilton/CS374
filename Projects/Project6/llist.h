// llist.h

#ifndef LLIST_H
#define LLIST_H

struct node {
    int size;    // size of the memory block
    int in_use;     // 1 if the memory block is in use, 0 if it is free
    struct node *next;
};

void llist_insert_head(struct node **head, struct node *n);
struct node *llist_delete_head(struct node **head);
void llist_insert_tail(struct node **head, struct node *n);
void llist_print(struct node *head);
void llist_free(struct node **head);

struct node *node_alloc(int size, int in_use);
void node_free(struct node *n);

#endif