#include <stdio.h>

union Pair {
  int left;
  int right;
};

static int pick_left(int value) {
  union Pair p;
  p.left = value;
  return p.left;
}

static int overwrite(int a, int b) {
  union Pair p;
  p.left = a;
  p.right = b;
  return p.left;
}

int main(void) {
  printf("%d\n", pick_left(7));
  printf("%d\n", overwrite(3, 9));
  return 0;
}
