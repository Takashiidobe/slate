#include <stdio.h>

int main(void) {
  float  x = 1.5f;
  double y = 2.25;
  printf("%f %.2f %.0f\n", x + 0.5f, y * 2.0, 3.0);
  printf("tail %.3f", 1.25);
  return 0;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: println!("{:.6} {:.2} {:.0}",
// REWRITES-DAG: print!("tail {:.3}",
