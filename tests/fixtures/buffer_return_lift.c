#include <stdio.h>
#include <stdlib.h>

static int *make(int n) {
  int *p = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    p[i] = i * i;
  }
  return p;
}

int main(void) {
  int *q = make(4);
  printf("%d %d\n", q[0], q[3]);
  free(q);
  return 0;
}
