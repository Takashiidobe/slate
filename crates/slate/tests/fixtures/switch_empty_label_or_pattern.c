#include <stdio.h>

int classify(int x) {
  switch (x) {
  case 1:;
  case 2:
    return 10;
  case 3: {
  }
  case 4: {
    ;
  }
  case 5:
    return 20;
  case 6:
    __attribute__((fallthrough));
  default:
    return 30;
  case -7:
    break;
  }
  return 40;
}

int stacked(unsigned long long x) {
  int out = 0;
  switch (x) {
  case 0:
  case 18446744073709551615ULL:
    out = 1;
    break;
  case 9223372036854775808ULL:
    out = 2;
  case 3:
    out += 3;
    break;
  }
  return out;
}

int main(void) {
  for (int i = -8; i <= 8; i++)
    printf("%d ", classify(i));
  printf("\n%d %d %d %d %d\n", stacked(0), stacked(18446744073709551615ULL),
         stacked(9223372036854775808ULL), stacked(3), stacked(4));
  return 0;
}
