#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "llist.h"

void *myalloc(int size);
void myfree(void *p);

#endif