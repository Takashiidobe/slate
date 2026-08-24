#include <stdio.h>

int classify(int n) {
  if (n < 0)
    goto neg;
  if (n == 0)
    goto zero;
  return 1;
neg:
  return -1;
zero:
  return 0;
}

int main() {
  printf("%d %d %d\n", classify(-5), classify(0), classify(42));
  return 0;
}
// REWRITES-DAG: if n < 0 {
// REWRITES-DAG: return -1;
// REWRITES-DAG: if n == 0 {
// REWRITES-DAG: return 0;
// REWRITES-NOT: __state0
// REWRITES-NOT: __dispatch0
