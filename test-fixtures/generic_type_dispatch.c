#include <stdio.h>

static int int_score(int value) { return value + 10; }
static int long_score(long value) { return (int)value + 20; }
static int pointer_score(const int *value) { return *value + 30; }

#define SCORE(value)                                                          \
  _Generic((value), int: int_score, long: long_score,                         \
           const int *: pointer_score)(value)

int main(void) {
  const int value = 7;
  int array[] = {5, 6};
  int first = SCORE(value);
  int second = SCORE(8L);
  int third = SCORE((const int *)array);
  printf("%d %d %d\n", first, second, third);
  return 0;
}
