#include <stdio.h>

int main(void) {
  printf("first %d\n", 1);
  printf("dynamic %*d\n", 5, 2);
  printf("last %d\n", 3);
  return 0;
}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES: std::io::Write::flush(&mut std::io::stdout()).unwrap();
// REWRITES: unsafe { printf(
// REWRITES: unsafe { fflush(
// REWRITES: println!("last {}", 3);
// REWRITES: {{^}}}
