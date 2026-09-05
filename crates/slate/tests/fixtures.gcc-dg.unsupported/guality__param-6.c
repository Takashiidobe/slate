/* { dg-do run { target i?86-*-* x86_64-*-* } } */
/* { dg-options "-g" } */

void __attribute__((noipa)) bar (void *p)
{}

void __attribute__((noipa)) foo (int i)
{
  void *p = __builtin_alloca (i);

  asm volatile ("" : : : "ebx");

  bar (p); /* { dg-final { gdb-test . "i" "5" } } */
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main (void)
{
  foo (5);
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end
