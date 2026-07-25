#include <stdio.h>
#include <stdlib.h>
static int cmp(const void *a, const void *b) {
  return *(const int *)a - *(const int *)b;
}
int main(void) {
  int a[5] = {1, 3, 5, 7, 9};
  int key = 7;
  int *p = bsearch(&key, a, 5, sizeof(int), cmp);
  printf("%d\n", p ? *p : -1);
  return 0;
}
