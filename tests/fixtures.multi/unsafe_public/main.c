#include <stdio.h>

int read_ptr(int *p);

int main(void) {
  int value = 7;
  printf("%d\n", read_ptr(&value));
  return 0;
}
