#include <stdio.h>

static int bump(int *n, int x) {
  *n = *n + 1;
  return x;
}

static int safe_ratio(int a, int b) { return b != 0 && a / b > 1; }

static int either_zero(int a, int b) { return a == 0 || b == 0 || 100 / a / b > 3; }

static int mixed(int a, int b, int c) { return (a || b) && c; }

static int grouped(int a, int b, int c) { return a && (b || c); }

static int negated(int a, int b) { return !(a && b) || !a; }

int main(void) {
  int n = 0, b = 0, i = 0;
  int r1 = n > 5 && bump(&n, 1);
  int r2 = n < 5 || bump(&n, 1);
  int r3 = bump(&n, 0) && (b = bump(&n, 7));
  int r4 = bump(&n, 0) || (b = bump(&n, 9));
  int r5 = (i++ > 0) && (i++ > 0);
  printf("%d %d %d %d %d n=%d b=%d i=%d\n", r1, r2, r3, r4, r5, n, b, i);

  if (bump(&n, 1) && bump(&n, 2)) printf("both n=%d\n", n);
  if (bump(&n, 0) || bump(&n, 0)) printf("unreachable\n");
  printf("n=%d\n", n);

  while (i < 10 && bump(&n, 1)) i++;
  printf("i=%d n=%d\n", i, n);

  printf("%d %d %d\n", safe_ratio(9, 0), safe_ratio(9, 2), safe_ratio(3, 2));
  printf("%d %d %d\n", either_zero(0, 5), either_zero(5, 0), either_zero(2, 3));
  printf("%d %d %d %d\n", mixed(0, 0, 1), mixed(0, 1, 1), mixed(1, 0, 0), mixed(1, 1, 1));
  printf("%d %d %d\n", grouped(0, 1, 1), grouped(1, 0, 1), grouped(1, 0, 0));
  printf("%d %d %d\n", negated(1, 1), negated(0, 1), negated(1, 0));
  return safe_ratio(4, 1) + mixed(1, 0, 1);
}
