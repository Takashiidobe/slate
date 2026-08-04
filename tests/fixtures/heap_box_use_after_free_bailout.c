#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *x = malloc(sizeof(int));
  free(x);
  *x = 4;
  printf("%d %d\n", *x, *x);
  return 0;
}
