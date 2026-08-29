#include <stdio.h>

int main() {
  int a = 3;
  int b = -1;
  int r;
  if (a > 0) {
    if (b > 0)
      goto both_pos;
    else
      goto a_pos;
  } else {
    goto other;
  }
both_pos:
  r = 1;
  goto done;
a_pos:
  r = 2;
  goto done;
other:
  r = 3;
  goto done;
done:
  printf("%d\n", r);
  return 0;
}
// REWRITES-DAG: r = 1;
// REWRITES-DAG: r = 2;
// REWRITES-DAG: r = 3;
