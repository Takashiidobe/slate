#include <stdio.h>

int adjust(_Atomic int value, int delta) {
  value = value + delta;
  return value;
}

int main(void) {
  printf("%d\n", adjust(4, 3));
  return 0;
}


