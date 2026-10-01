#include <stdio.h>

static int total = 100;

static void fill(int *out, int value) { *out = value; }

static void fill_pair(long *first, long *second) {
  *first = 11;
  *second = 22;
}

static int shadow_params(int value, int step) {
  int result = value;
  {
    int value = step * 10;
    result += value;
    {
      int step = value + 1;
      result += step;
    }
  }
  return result + value + step;
}

int main(void) {
  int x = 1;
  int outer_seen = 0;
  {
    int outer = x;
    int x = outer * 5;
    outer_seen = x;
    {
      int x = 40;
      outer_seen += x;
    }
    outer_seen += x;
  }
  for (int i = 0; i < 2; i++) {
    int x = i + 7;
    outer_seen += x;
  }
  {
    int sibling = 3;
    outer_seen += sibling;
  }
  {
    int sibling = 4;
    outer_seen += sibling;
  }

  int first = 2, second = first * 3, third = second + first;
  int total = third + 1;

  int type = 1, match = 2, loop = 3, ref = 4, fn = 5, impl = 6, mod = 7;
  int keywords = type + match * loop + ref * fn + impl - mod;

  int filled;
  fill(&filled, 77);
  long a, b;
  fill_pair(&a, &b);
  int chosen;
  if (x > 0) {
    chosen = 10;
  } else {
    chosen = 20;
  }

  int sum = 0;
  for (int round = 0; round < 3; round++) {
    int fresh = 1;
    fresh += round;
    sum += fresh;
  }

  int counter = 0;
  while (counter < 3) {
    int step;
    step = counter * 2;
    counter++;
    sum += step;
  }

  printf("%d %d %d %d %d\n", x, outer_seen, first, second, third);
  printf("%d %d %d\n", total, keywords, shadow_params(2, 3));
  printf("%d %ld %ld %d %d\n", filled, a, b, chosen, sum);
  return (total + x) & 0x3f;
}

int read_static_total(void) { return total; }
