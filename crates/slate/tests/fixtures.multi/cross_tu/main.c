#include <stdio.h>

int square(int x);
int cube(int x);

int main(void) {
  printf("%d %d\n", square(6), cube(4));
  return 0;
}
// LOWERING-DAG: mod math;
// LOWERING-DAG: use crate::math::square;
// LOWERING-DAG: use crate::math::cube;
// LOWERING-NOT: fn square
// LOWERING-NOT: fn cube
