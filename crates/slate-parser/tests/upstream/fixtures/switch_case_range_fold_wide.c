#include <stdio.h>

typedef _BitInt(128) big;

int classify_u64(unsigned long long v) {
  switch (v) {
  case 18446744073709551610ULL:
  case 18446744073709551611ULL:
  case 18446744073709551612ULL:
    return 1;
  case 18446744073709551615ULL:
    return 2;
  default:
    return 0;
  }
}

int classify_bitint(int seed) {
  big v = (big)170141183460469231731687303715884105720wb + (big)seed;
  switch (v) {
  case 170141183460469231731687303715884105720wb:
  case 170141183460469231731687303715884105721wb:
  case 170141183460469231731687303715884105722wb:
    return 1;
  case 170141183460469231731687303715884105727wb:
    return 2;
  default:
    return 0;
  }
}

int main(void) {
  printf("%d %d %d %d %d %d\n", classify_u64(18446744073709551611ULL),
         classify_u64(18446744073709551615ULL), classify_u64(3),
         classify_bitint(1), classify_bitint(7), classify_bitint(4));
  return 0;
}


