#include <stddef.h>
#include <stdio.h>
int main(void) {
  int arr[5] = {0, 1, 2, 3, 4};
  int *p = &arr[1];
  int *q = &arr[4];
  ptrdiff_t d = q - p;
  size_t n = sizeof(arr) / sizeof(arr[0]);
  printf("%d %d\n", (int)d, (int)n);
  return 0;
}
