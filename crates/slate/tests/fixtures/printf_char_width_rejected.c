#include <stdio.h>

int main(void) {
  int width = 3;
  printf("%*c\n", width, 'a');
  return 0;
}
// REWRITES-DAG: fn printf(
// REWRITES-DAG: unsafe { printf(
// REWRITES-NOT: println!(
