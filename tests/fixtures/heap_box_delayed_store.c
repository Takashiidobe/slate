#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int));
  int marker = 7;
  marker = marker + 1;
  *p = 41;
  printf("%d %d\n", *p, marker);
  free(p);
  return 0;
}
