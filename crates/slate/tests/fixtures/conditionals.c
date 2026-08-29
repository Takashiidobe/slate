#include <stdio.h>

static int classify(int n) {
  if (n < 0) {
    return -1;
  } else if (n == 0) {
    return 0;
  } else {
    return 1;
  }
}

static int clamp_low(int n) {
  int r = n;
  if (n < 10) {
    r = 10;
  }
  return r;
}

int main(void) {
  printf("%d\n", classify(-5));
  printf("%d\n", classify(0));
  printf("%d\n", classify(42));
  printf("%d\n", clamp_low(3));
  printf("%d\n", clamp_low(20));

  int x = 7;
  int label;
  if (x % 2 == 0) {
    label = 100;
  } else {
    label = 200;
  }
  printf("%d\n", label);
  return 0;
}
// REWRITES-LABEL: {{^}}fn clamp_low(
// REWRITES-NOT: {{^    \{$}}
// REWRITES: {{^}}}
