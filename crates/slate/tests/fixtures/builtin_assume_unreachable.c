#include <stdio.h>

static int classify(int x) {
  switch (x & 3) {
  case 0:
    return 10;
  case 1:
    return 11;
  case 2:
    return 12;
  case 3:
    return 13;
  }
  __builtin_unreachable();
}

static int positive_half(int x) {
  __builtin_assume(x > 0);
  return x / 2;
}

static int bump(int *counter) {
  return ++*counter;
}

static unsigned checked(unsigned x) {
  if (x < 100)
    return x * 3;
  __builtin_unreachable();
}

int main(void) {
  int counter = 0;
  int x = 5;
  __builtin_assume(x++ > 0);
  __builtin_assume(bump(&counter) > 0);
  printf("%d %d %d\n", x, counter, positive_half(9));
  for (int i = 0; i < 6; i++)
    printf("%d ", classify(i));
  printf("\n%u\n", checked(7));
  return 0;
}
