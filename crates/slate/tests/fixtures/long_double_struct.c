#include <stdio.h>

struct box {
  int tag;
  long double value;
};

static int sum_box(struct box b) { return (int)(b.value + (long double)b.tag); }

int main(void) {
  struct box b;
  b.tag = 3;
  b.value = 4.5L;
  printf("%d\n", sum_box(b));
  b.value = b.value * 2.0L;
  printf("%d\n", (int)b.value);
  return 0;
}
