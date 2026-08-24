#include <stdio.h>

int main() {
  int n = 7;
  int cls;
  if (n < 0) {
    goto neg;
  } else {
    goto nonneg;
  }
neg:
  cls = -1;
  goto print;
nonneg:
  cls = 1;
  goto print;
print:
  printf("%d\n", cls);
  return 0;
}
// REWRITES-DAG: if n < 0 {
// REWRITES-DAG: cls = -1;
// REWRITES-DAG: cls = 1;
// REWRITES-DAG: println!("{}", cls);
// REWRITES-NOT: __state0
// REWRITES-NOT: __dispatch0
