#include <stdio.h>

int main(void) {
  int      d = 5;
  unsigned h = 0xAB;
  printf("{%d} %% \"quoted\" back\\slash %s|%c|%x\n", d, "hi", 'X', h);
  printf("}}%%{{%d}}\n", d);
  printf("%%%%%d%%%%\n", d);
  printf("{{}}%s{{}}\n", "mid");
  return 0;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: quoted
// REWRITES-DAG: back\\slash
// REWRITES-DAG: println!("%%{}%%", d);
