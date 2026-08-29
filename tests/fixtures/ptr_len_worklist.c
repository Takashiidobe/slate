#include <stdio.h>
#include <stdlib.h>

static void fill_values(int *values, int len) {
  for (int i = 0; i < len; ++i)
    values[i] = i * 3;
}

static void forward_fill(int *values, int len) {
  fill_values(values, len);
}

int main(void) {
  int len = 5;
  int *values = malloc(len * sizeof(int));
  forward_fill(values, len);
  printf("%d %d\n", values[1], values[4]);
  return 0;
}
