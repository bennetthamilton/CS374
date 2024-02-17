#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "llist.h"

void *mymalloc(int size);
void myfree(void *p);

#endif