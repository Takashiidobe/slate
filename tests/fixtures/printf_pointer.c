#include <stdint.h>
#include <stdio.h>

int main(void) {
  void *stable = (void *)(uintptr_t)0x1234;
  printf("%p\n", stable);
  printf("addr=%p", (void *)(uintptr_t)0xABCD);
  return 0;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: println!("{:p}",
// REWRITES-DAG: print!("addr={:p}",
