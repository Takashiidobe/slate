#include <stdio.h>
#include <stdlib.h>

int *alloc(void) { return malloc(sizeof(int) * 10); }

int *indirect(void) { return alloc(); }

int main(void) {
  int *x = NULL;
  x      = indirect();
  x[0]   = 10;
  printf("%d\n", x[0]);
  free(x);
  return 0;
}
