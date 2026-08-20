#include <stdio.h>

static int classify(_BitInt(200) c) {
  switch (c) {
  case 1:
  case 2:
    break;
  case 123456789012345678901234567890123456789012345wb:
    goto low;
  case 4:
    goto high;
  default:
    return -1;
  }
  for (;;) {
    switch (c) {
    case 5:
    low:
      return 100;
    case 6:
    high:
      return 200;
    default:
      return -2;
    }
  }
}

int main() {
  printf("%d\n", classify(1));
  printf("%d\n", classify(123456789012345678901234567890123456789012345wb));
  printf("%d\n", classify(4));
  printf("%d\n", classify(5));
  printf("%d\n", classify(6));
  printf("%d\n", classify(9));
  return 0;
}
