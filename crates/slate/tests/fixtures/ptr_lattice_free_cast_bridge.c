#include <stdio.h>
#include <stdlib.h>

static void just_free(int *p) { free(p); }

int main(void) {
  int *x = malloc(sizeof(int));
  *x = 4;
  printf("%d\n", *x);
  just_free(x);
  return 0;
}
