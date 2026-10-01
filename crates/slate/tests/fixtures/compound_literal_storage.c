#include <stdio.h>

struct Point {
  int x;
  int y;
};

struct Pair {
  struct Point *first;
  int          *values;
};

static struct Point *origin = &(struct Point){3, 4};
static int          *primes = (int[]){2, 3, 5, 7};
static struct Pair   pair   = {&(struct Point){5, 6}, (int[]){8, 9}};

static int sum(const int *values, int count) {
  int total = 0;
  for (int i = 0; i < count; i++)
    total += values[i];
  return total;
}

int main(void) {
  origin->x += 10;
  primes[3] = 11;
  printf("%d %d %d %d\n", origin->x, origin->y, primes[3], sum(primes, 4));
  printf("%d %d %d\n", pair.first->x, pair.first->y, pair.values[1]);

  for (int i = 0; i < 3; i++) {
    int *counter = &(int){i * 10};
    *counter += 1;
    printf("%d ", *counter);
  }
  printf("\n");

  struct Point copied = (struct Point){.y = 7};
  int          total  = sum((int[]){1, 2, 3, copied.y}, 4);
  printf("%d %d %d\n", copied.x, copied.y, total);
  return 0;
}
