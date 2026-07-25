#include <stdio.h>

int counter = 4;
int zeroed;
int numbers[4] = {1, 2};

struct Pair {
  int left;
  int right;
};

struct Pair pair = {3, 5};

static int adjust(int by) {
  counter += by;
  zeroed += counter;
  numbers[2] = zeroed - numbers[0];
  pair.right += numbers[1];
  return pair.left + pair.right;
}

int main(void) {
  printf("%d\n", adjust(6));
  printf("%d %d %d\n", counter, zeroed, numbers[2]);
  return 0;
}
