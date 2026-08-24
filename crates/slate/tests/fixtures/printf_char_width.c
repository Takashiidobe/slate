#include <stdio.h>

int main(void) {
  printf("%3c|%-3c|%c\n", 'a', 'b', 'c');
  return 0;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: println!("{}|{}|{}", "  a", "b  ", "c");
