/* Verify that we do not lose side effects on a MOD expression.  */

#include <stdio.h>
#include <stdlib.h>

int foo(int a) {
  int x = 0 % a++;
  return a;
}

int main(void) {
  if (foo(9) != 10)
    abort();
  exit(0);
}
