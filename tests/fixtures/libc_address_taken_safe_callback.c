#include <stdio.h>
#include <stdlib.h>

typedef void *(*ReallocFn)(void *, size_t);
typedef void (*FreeFn)(void *);

struct Allocator {
  ReallocFn realloc_fn;
  FreeFn free_fn;
};

struct Allocator alloc = {realloc, free};

int main(void) {
  void *p = alloc.realloc_fn(NULL, 8);
  alloc.free_fn(p);
  printf("ok\n");
  return 0;
}
