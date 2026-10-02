#include <stdio.h>

static int calls;
static int choose(int value) {
  calls++;
  return value % 4;
}

static void accumulate(int n, int *total) {
  int i = 0;
again:
  if (i >= n)
    goto done;
  switch (choose(i++)) {
  case 0: *total += 3; break;
  case 1: *total += 5;
  case 2: *total += 7; break;
  default: *total -= 2; break;
  }
  goto again;
done:
  *total += calls;
}

static int classify(const char *s) {
again:
  switch (*s) {
  case 'a': return 1;
  case 'b': return 2;
  case 'c': return 3;
  case 'd': return 4;
  case ' ': s++; goto again;
  case '\t': s++; goto again;
  case 'n': return 5;
  default: return 6;
  }
}

int main(void) {
  int total = 0;
  for (int n = 0; n < 12; n++) {
    calls = 0;
    accumulate(n, &total);
    printf("%d %d %d\n", n, calls, total);
  }
  printf("%d %d %d %d\n", classify("  a"), classify("\t b"),
         classify("  n"), classify(" x"));
  return 0;
}
