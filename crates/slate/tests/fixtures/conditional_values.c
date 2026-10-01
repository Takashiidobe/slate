#include <stdio.h>

struct Pair {
  int a;
  int b;
};

static int sign(int x) { return x > 0 ? 1 : x < 0 ? -1 : 0; }

static int clamp_add(int x, int y) { return (x > y ? x : y) * 2 + (x < y ? x : y); }

static const char *pick(int which, const char *left, const char *right) {
  return which ? left : right;
}

static int sum_pair(struct Pair pair) { return pair.a * 10 + pair.b; }

int main(void) {
  int values[] = {5, -7, 0, 12};
  int total = 0;
  for (int i = 0; i < 4; i++)
    total += sign(values[i]) * 100 + (values[i] % 2 ? 7 : 3);
  struct Pair first = {1, 2};
  struct Pair second = {3, 4};
  int x = 4, y = 9;
  int seq = x + y;
  int nested = (x > 3 ? (y < 5 ? 1 : 2) : 3) + (y > 8);
  double half = x > y ? x / 2.0 : y / 2.0;
  unsigned long wide = y > 0 ? 4000000000UL : 1UL;
  int buffer[3] = {10, 20, 30};
  int *cursor = x ? &buffer[2] : &buffer[0];
  if (x > 2 ? y > 2 : y < 2)
    total += 1000;
  printf("%d %d %d %d\n", total, clamp_add(3, 8), clamp_add(9, -1), seq);
  printf("%d %.1f %lu %d\n", nested, half, wide, *cursor);
  printf("%s %s\n", pick(1, "left", "right"), pick(0, "left", "right"));
  printf("%d\n", sum_pair(y > x ? first : second));
  return sign(-x) + 1;
}
