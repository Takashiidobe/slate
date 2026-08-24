#include <stdio.h>

int score(int x) {
  int out = 0;
  switch (x) {
  case 1:
    out += 10;
  case 2:
    out += 20;
    break;
  case 3:
  case 4:
    out += 40;
    break;
  default:
    out += 90;
  }
  return out;
}

int main(void) {
  printf("%d %d %d %d %d\n", score(1), score(2), score(3), score(4), score(8));
  return 0;
}
// REWRITES-DAG: match __switch_case0 {
// REWRITES-DAG: 0 => {
// REWRITES-NOT: _ => break '__switch0,
