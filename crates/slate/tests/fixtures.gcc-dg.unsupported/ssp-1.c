/* { dg-do run } */
/* { dg-options "-fstack-protector" } */
/* { dg-require-effective-target fstack_protector } */

#include <stdlib.h>

void __stack_chk_fail(void) { exit(0); /* pass */ }

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
  register int i;
  char         foo[255];

  // smash stack
  for (i = 0; i <= 400; i++)
    foo[i] = 42;

  return 1; /* fail */
}
// @rewrite-fn-end
// @lowering-fn-end
