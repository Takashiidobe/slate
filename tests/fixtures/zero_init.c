#include <stdio.h>

int main(void) {
  int x = 0;
  x     = 42;

  int y = 0;
  int z = getchar();
  y     = z + 1;

  printf("%d %d %d\n", x, y, z);
  return 0;
}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: let x: i32 = 42;
// REWRITES-DAG: let z: i32 = unsafe { getchar() };
// REWRITES-DAG: let y: i32 = z + 1;
// REWRITES-NOT: let mut x
// REWRITES-NOT: let mut y
// REWRITES: {{^}}}
