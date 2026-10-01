#include <stdio.h>

static int classify(int value) {
  int result = 0;
  switch (value) {
  case 1 ... 4:
    result += 10;
  case 5 ... 8:
    result += 20;
    break;
  case 10 ... 12:
    result += 40;
    break;
  default:
    result = 90;
  }
  return result;
}

static int classify_direct(int value) {
  switch (value) {
  case -2 ... 2:
    return 7;
  default:
    return 9;
  }
}

static int classify_empty(int value) {
  switch (value) {
  case 5 ... 3:
    return 1;
  case 4:
    return 2;
  }
  return 3;
}

int main(void) {
  printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", classify(1), classify(2),
         classify(4), classify(5), classify(7), classify(8), classify(9),
         classify(10), classify(11), classify(12), classify_direct(-1),
         classify_direct(3));
  printf("%d %d %d\n", classify_empty(3), classify_empty(4), classify_empty(5));
  return 0;
}
