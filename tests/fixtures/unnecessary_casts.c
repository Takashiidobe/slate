#include <stdio.h>

int main(void) {
  unsigned char a = 200;
  unsigned char b = 100;
  unsigned char c = 0;
  for (int i = 0; i < 1; i++) {
    c = a + b;
  }
  printf("%d\n", c);
  return 0;
}
// REWRITES-DAG: c = a + b;
// REWRITES-NOT: a as i32
// REWRITES-NOT: b as i32
