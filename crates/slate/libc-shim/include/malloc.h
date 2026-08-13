#ifndef _SLATE_MALLOC_H
#define _SLATE_MALLOC_H

#include <features.h>

#include <stddef.h>
#include <stdio.h>

void *malloc(size_t);
void *calloc(size_t, size_t);
void *realloc(void *, size_t);
void *reallocarray(void *, size_t, size_t);
void  free(void *);
void *memalign(size_t, size_t);
void *valloc(size_t);
void *pvalloc(size_t);

struct mallinfo {
  int arena;
  int ordblks;
  int smblks;
  int hblks;
  int hblkhd;
  int usmblks;
  int fsmblks;
  int uordblks;
  int fordblks;
  int keepcost;
};

struct mallinfo2 {
  size_t arena;
  size_t ordblks;
  size_t smblks;
  size_t hblks;
  size_t hblkhd;
  size_t usmblks;
  size_t fsmblks;
  size_t uordblks;
  size_t fordblks;
  size_t keepcost;
};

struct mallinfo  mallinfo(void);
struct mallinfo2 mallinfo2(void);

#define M_MXFAST 1
#define M_NLBLKS 2
#define M_GRAIN  3
#define M_KEEP   4

#define M_TRIM_THRESHOLD -1
#define M_TOP_PAD        -2
#define M_MMAP_THRESHOLD -3
#define M_MMAP_MAX       -4
#define M_CHECK_ACTION   -5
#define M_PERTURB        -6
#define M_ARENA_TEST     -7
#define M_ARENA_MAX      -8

int    mallopt(int, int);
int    malloc_trim(size_t);
size_t malloc_usable_size(void *);
void   malloc_stats(void);
int    malloc_info(int, FILE *);

#endif
