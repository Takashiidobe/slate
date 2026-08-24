#include <stdio.h>

int main(void) {
  printf("%s %c %c %d\n", "tag", 'A', 10, 7);
  printf("literal=%s", "tail");
  return 0;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: println!("{} {} {} {}", "tag", "A", "\n", 7);
// REWRITES-DAG: print!("literal={}", "tail");
