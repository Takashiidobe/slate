#include <stdio.h>

typedef _BitInt(256) big;

int classify(int seed) {
  big s = 170141183460469231731687303715884105727wb + (big)seed;
  switch (s) {
  case 170141183460469231731687303715884105727wb ... 170141183460469231731687303715884105730wb:
    return 1;
  case 170141183460469231731687303715884105740wb:
  case 170141183460469231731687303715884105745wb ... 170141183460469231731687303715884105747wb:
    return 2;
  default:
    return 0;
  }
}

int probe(int seed) {
  big s = 170141183460469231731687303715884105727wb + (big)seed;
  int v = 42;
  switch (s) {
  case 170141183460469231731687303715884105727wb ... 170141183460469231731687303715884105730wb:
    return v;
  default:
    return 0;
  }
}

int main(void) {
  printf("%d %d %d %d %d %d\n", classify(0), classify(13), classify(19),
         classify(5), probe(2), probe(9));
  return 0;
}


