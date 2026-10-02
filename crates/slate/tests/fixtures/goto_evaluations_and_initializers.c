#include <stdio.h>

static int initializers;
static int initialize(void) { ++initializers; return 19; }

static int run(int mode) {
  int sum = 0;
  int checks = 0;
  int steps = 0;
  int i = 0;
  if (mode) goto entered;
  {
    int value = initialize();
  entered:
    value = 3;
    sum += value;
  }
  for (i = 0; ++checks && i < 4; ++steps, ++i) {
  retry:
    if (i == 0) { i = 1; goto retry; }
    if (i == 2) continue;
    sum += i;
  }
  do {
    ++sum;
    if (sum < 10) continue;
    break;
  } while (++checks < 20);
  switch (mode) {
    int skipped = initialize();
    case 1:
      sum += 2;
    case 2 ... 4:
      sum += 3;
      break;
    default:
      sum += 4;
  }
  printf("%d %d %d %d\n", sum, checks, steps, initializers);
  return sum;
}

int main(void) {
  run(1);
  run(0);
  return 0;
}
