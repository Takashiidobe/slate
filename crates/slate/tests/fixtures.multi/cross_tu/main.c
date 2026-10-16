#include <stdio.h>

int square(int x);
int cube(int x);

// @check-fn-begin
int main(void) {
  printf("%d %d\n", square(6), cube(4));
  return 0;
}
// @check-fn-end
