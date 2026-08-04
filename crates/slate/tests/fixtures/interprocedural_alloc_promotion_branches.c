#include <stdio.h>
#include <stdlib.h>

int *alloc(void) { return malloc(sizeof(int) * 10); }

int *a(void) { return alloc(); }

int *b() {
  if (1)
    return a();
  return alloc();
}

int *c(void) { return b(); }

int main(void) {
  int *x = NULL;
  x = c();
  x[0] = 10;
  printf("%d\n", x[0]);
  free(x);
  return 0;
}
