#include <stdio.h>

int main(void) {
  char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  __builtin_bcopy(buf, buf + 2, 4);
  for (int i = 0; i < 8; i++)
    printf("%d ", buf[i]);
  printf("\n");
  return 0;
}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: buf.copy_within(0..4, 2);
// REWRITES-NOT: std::ptr::copy
// REWRITES-NOT: buf.as_mut_ptr().add(2)
// REWRITES: {{^}}}
