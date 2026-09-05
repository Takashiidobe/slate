/* { dg-do run } 
 * { dg-options "-std=c99" }
 * */


static char tmp[2];

static int f(int n, char (*x)[sizeof (*(++n, (char (*)[n])&tmp))])
{
  return sizeof *x;
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main (void)
{
  if (2 != f(1, &tmp))
    __builtin_abort ();
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

