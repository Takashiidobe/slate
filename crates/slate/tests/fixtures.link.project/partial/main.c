#include <stdio.h>

extern int compute_b(int x);
extern int native_c(int x);

int main(void) {
  printf("%d\n", compute_b(5) + native_c(2));
  return 0;
}
