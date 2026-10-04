#include <stdio.h>

static int total(const int *values, int count) {
  int sum = 0;
  for (int i = 0; i < count; i++)
    sum += values[i];
  return sum;
}

int main(void) {
  int values[] = {3, 5, 7, 11};
  printf("%d\n", total(values, 4));
  return 0;
}
