#include <stdio.h>
#include <stdlib.h>

int main(void) {
  void *p = malloc(16);
  free_sized(p, 16);

  void *q = aligned_alloc(16, 32);
  free_aligned_sized(q, 16, 32);

  void *r = realloc(NULL, 0);
  free(r);

  void *a = aligned_alloc(64, 64);
  printf("%d\n", memalignment(a) % 64 == 0);
  free(a);

  printf("ok\n");
  return 0;
}
