#include <stdio.h>
#include <stdlib.h>

static unsigned long runtime_alloc_size(int n) {
  void         *p    = malloc(n);
  unsigned long size = __builtin_dynamic_object_size(p, 0);
  free(p);
  return size;
}

int main(void) {
  int  small[3];
  int *p = small;

  printf("%lu %lu %lu %lu\n", (unsigned long)__builtin_object_size(p, 0),
         (unsigned long)__builtin_object_size(p, 1),
         (unsigned long)__builtin_object_size(p, 2),
         (unsigned long)__builtin_object_size(p, 3));
  printf("%lu %lu\n", (unsigned long)__builtin_object_size(p + 1, 0),
         (unsigned long)__builtin_dynamic_object_size(p + 2, 2));

  char *m = malloc(37);
  printf("%lu %lu %lu %lu\n", (unsigned long)__builtin_object_size(m, 0),
         (unsigned long)__builtin_object_size(m, 1),
         (unsigned long)__builtin_object_size(m + 5, 2),
         (unsigned long)__builtin_object_size(m, 3));
  free(m);

  printf("%lu\n", runtime_alloc_size(41));
  return 0;
}
