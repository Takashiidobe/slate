#ifndef _SLATE_MALLOC_H
#define _SLATE_MALLOC_H

#include <features.h>

#define __NEED_size_t
#include <bits/types.h>

void *malloc(size_t);
void *calloc(size_t, size_t);
void *realloc(void *, size_t);
void  free(void *);
void *valloc(size_t);
void *memalign(size_t, size_t);

size_t malloc_usable_size(void *);

#endif
