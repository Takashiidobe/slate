#include <stdio.h>
enum FuzzEnum {
  FuzzZero,
  FuzzOne,
  FuzzFive = 5,
  FuzzSix,
  FuzzNegative = -2,
  FuzzNegativeNext
};
union FuzzPair {
  int left;
  int right;
};
static int helper(int a, int b) {
  int c = a + b;
  return c;
}
static int loop_sum(int n) {
  int total = 0;
  for (int i = 1; i <= n; i++) {
    total += i;
  }
  return total;
}
static int array_pick(int index) {
  int values[3];
  values[0] = 6;
  values[1] = 1;
  values[2] = values[0] + values[1];
  return values[index];
}
static int union_pick(int a, int b) {
  union FuzzPair p;
  p.left = a;
  p.right = b;
  return p.left;
}
int main(void) {
  printf("%d\n", helper(7, 2));
  printf("%d\n", loop_sum(2));
  printf("%d\n", array_pick(1));
  printf("%d\n", union_pick(8, 9));
  printf("%d\n", FuzzSix);
  return 0;
}
