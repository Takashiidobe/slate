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
// REWRITES-DAG: if !(i < 5) {
// REWRITES-DAG: sum += i;
// REWRITES-DAG: i += 1;
// REWRITES-DAG: println!("{}", sum);
// REWRITES-NOT: __state0
// REWRITES-NOT: __dispatch0
// REWRITES-NOT: __retval
