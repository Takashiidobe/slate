#include <stdio.h>
#include <stdlib.h>

int *alloc(void) { return malloc(sizeof(int) * 10); }

int main(void) {
  int *x = NULL;
  x      = alloc();
  x[0]   = 10;
  printf("%d\n", x[0]);
  free(x);
  return 0;
}
