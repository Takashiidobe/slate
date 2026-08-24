#include <stdio.h>

static int sum_while(int n) {
  int total = 0;
  int i     = 1;
  while (i <= n) {
    total += i;
    i++;
  }
  return total;
}

int main(void) {
  printf("%d\n", sum_while(5));
  printf("%d\n", sum_while(1));
  printf("%d\n", sum_while(0));
  return 0;
}
// REWRITES-LABEL: {{^}}fn sum_while(
// REWRITES-DAG: loop {
// REWRITES-DAG: if !(i <= n) {
// REWRITES-DAG: total += i;
// REWRITES-DAG: i += 1;
// REWRITES-NOT: {{^        \{$}}
// REWRITES: {{^}}}
