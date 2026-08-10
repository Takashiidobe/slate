#include <stdio.h>

int classify(int x, int extra) {
  int result = -1;
  switch (x) {
  case 0:
    if (extra) {
      result = 10;
      break;
    }
    result = 11;
    break;
  case 1:
    result = 20;
    break;
  default:
    result = -2;
    break;
  }
  return result;
}

int main(void) {
  printf("%d\n", classify(0, 1));
  printf("%d\n", classify(0, 0));
  printf("%d\n", classify(1, 0));
  printf("%d\n", classify(5, 0));
  return 0;
}
