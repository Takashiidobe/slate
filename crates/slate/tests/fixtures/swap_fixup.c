#include <stdio.h>

static void basic_swap(void) {
  int a = 1;
  int b = 2;
  int tmp = a;
  a = b;
  b = tmp;
  printf("%d %d\n", a, b);
}

static void tmp_reused(void) {
  int a = 3;
  int b = 4;
  int tmp = a;
  a = b;
  b = tmp;
  printf("%d %d %d\n", a, b, tmp);
}

static void self_swap(void) {
  int a = 5;
  int tmp = a;
  a = a;
  a = tmp;
  printf("%d\n", a);
}

int main(void) {
  basic_swap();
  tmp_reused();
  self_swap();
  return 0;
}
