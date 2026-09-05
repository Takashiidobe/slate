/* { dg-do run }  */
/* { dg-require-ifunc "" } */
/* { dg-options "" } */

#include <stdio.h>

static int implementation (void)
{
  printf ("'ere I am JH\n");
  return 0;
}

static __typeof__ (implementation)* resolver (void)
{
  return implementation;
}

static int magic (void) __attribute__ ((ifunc ("resolver")));

// @lowering-fn-begin
// @rewrite-fn-begin
int main ()
{
  return magic () != 0;
}
// @rewrite-fn-end
// @lowering-fn-end
