#include <stdio.h>

int main(void) {
  int a[5] = {1, 2, 3, 4, 5};
  __builtin_bzero(a, sizeof(a));
  int sum = 0;
  for (int i = 0; i < 5; i++)
    sum += a[i];
  printf("%d\n", sum);
  return 0;
}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: a.fill(0);
// REWRITES-NOT: std::ptr::write_bytes
// REWRITES: {{^}}}
