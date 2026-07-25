#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = aligned_alloc(16, 4 * sizeof(int));
  if (!p) {
    puts("alloc failed");
    return 1;
  }
  p[0] = 3;
  p[1] = 4;
  p[2] = 5;
  p[3] = 6;
  printf("%d %d\n", ((unsigned long)p) % 16 == 0, p[0] + p[3]);
  free(p);
  return 0;
}
