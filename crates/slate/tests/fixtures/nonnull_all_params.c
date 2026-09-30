#include <stdio.h>

__attribute__((nonnull)) int use_all(int *a, int *b) { return *a - *b; }

int main(void) {
  int x = 10;
  int y = 4;
  printf("%d\n", use_all(&x, &y));
  return 0;
}
