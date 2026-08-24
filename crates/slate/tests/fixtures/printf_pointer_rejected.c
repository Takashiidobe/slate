#include <stdint.h>
#include <stdio.h>

int main(void) {
  void *stable = (void *)(uintptr_t)0x1234;
  printf("%20p\n", stable);
  return 0;
}
// REWRITES-DAG: fn printf(
// REWRITES-DAG: unsafe { printf(
// REWRITES-NOT: println!("{:p}
