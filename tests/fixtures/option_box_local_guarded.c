#include <stdio.h>
#include <stdlib.h>

int compute(int flag) {
  int *p = NULL;
  if (flag) {
    p = malloc(sizeof(int));
  }
  if (p) {
    *p    = 41;
    int v = *p;
    printf("%d\n", v);
    free(p);
  } else {
    printf("no alloc\n");
  }
  return 0;
}

int main(void) {
  compute(1);
  compute(0);
  return 0;
}
