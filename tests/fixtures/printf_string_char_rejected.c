#include <stdio.h>

int main(void) {
  char buf[4] = "hey";
  printf("%s\n", buf);
  return 0;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: let buf: &str = "hey";
// REWRITES-DAG: println!("{}", buf);
