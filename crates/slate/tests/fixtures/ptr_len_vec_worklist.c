#include <stdio.h>
#include <stdlib.h>

static int consume_values(int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i) {
    values[i] += 1;
    sum += values[i];
  }
  free(values);
  return sum;
}

static int forward_consume(int *values, int len) {
  return consume_values(values, len);
}

int main(void) {
  int len = 4;
  int *values = malloc(len * sizeof(int));
  for (int i = 0; i < len; ++i)
    values[i] = i * 2;
  int sum = forward_consume(values, len);
  printf("%d\n", sum);
  return 0;
}

// REWRITES-DAG: fn consume_values(mut arg{{[0-9]+}}: Vec<i32>) -> i32
// REWRITES-DAG: fn forward_consume(mut arg{{[0-9]+}}: Vec<i32>) -> i32
// REWRITES: Vec::from_raw_parts(
// REWRITES-NOT: unsafe { free(
