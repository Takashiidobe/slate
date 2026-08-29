#include <stdio.h>

int main() {
  int i   = 0;
  int sum = 0;
loop:
  sum = sum + i;
  i   = i + 1;
  if (i < 5)
    goto loop;
  printf("%d\n", sum);
  return 0;
}
// REWRITES-DAG: loop {
