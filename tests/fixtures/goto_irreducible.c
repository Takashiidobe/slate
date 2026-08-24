#include <stdio.h>

int main() {
  volatile int choose_b = 0;
  int          x        = 0;
  if (choose_b)
    goto b;
a:
  x = x + 1;
  if (x < 3)
    goto b;
  goto done;
b:
  x = x + 2;
  if (x < 4)
    goto a;
done:
  printf("%d\n", x);
  return 0;
}
// REWRITES-DAG: let mut __block0: i32;
// REWRITES-DAG: match __block0 {
// REWRITES-DAG: __block0 = 2;
// REWRITES-DAG: __block0 = 1;
// REWRITES-DAG: println!("{}", x);
// REWRITES-NOT: __state0
// REWRITES-NOT: __dispatch0
// REWRITES-NOT: {{^    x = 0;$}}
