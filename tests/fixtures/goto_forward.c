#include <stdio.h>

int main() {
  int x = 1;
  goto done;
  x = 99;
done:
  printf("%d\n", x);
  return 0;
}
// REWRITES-DAG: let x: i32 = 1;
// REWRITES-DAG: println!("{}", x);
// REWRITES-NOT: __state0
// REWRITES-NOT: __dispatch0
// REWRITES-NOT: x = 99;
