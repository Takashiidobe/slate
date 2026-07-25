#include <stdio.h>
#include <stdlib.h>

static int cmp_int(const void *a, const void *b) {
  return *(const int *)a - *(const int *)b;
}

int main(void) {
  int nums[5] = {4, 1, 5, 3, 2};
  qsort(nums, 5, sizeof(int), cmp_int);
  printf("%d %d %d\n", nums[0], nums[2], nums[4]);
  return 0;
}
