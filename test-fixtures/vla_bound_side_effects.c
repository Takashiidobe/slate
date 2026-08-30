#include <stdio.h>

static int parameter_bound(int length, int values[length++]) {
  return length + values[0];
}

int main(void) {
  int bound = 3;
  unsigned long evaluated = sizeof(int[bound++]);
  unsigned long unevaluated = sizeof(int(*)[bound++]);
  int values[] = {7, 8, 9};
  int parameter = parameter_bound(3, values);
  printf("%lu %lu %d %d\n", evaluated, unevaluated, bound, parameter);
  return 0;
}
