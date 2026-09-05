/* PR tree-optimization/121131 */
/* { dg-do run { target bitint } } */
/* { dg-options "-O2" } */

#if __BITINT_MAXWIDTH__ >= 156
struct A { _BitInt(156) b : 135; };

static inline _BitInt(156)
foo (struct A *x)
{
  return x[1].b;
}

__attribute__((noipa)) _BitInt(156)
bar (void)
{
  struct A a[] = { 1, 1, -13055525270329736316393717310914023773847wb,
		   1, 1, 1, 1, 1, 1, 1, 1, 1 };
  return foo (&a[1]);
}
#endif

// @lowering-fn-begin
// @rewrite-fn-begin
int
main ()
{
#if __BITINT_MAXWIDTH__ >= 156
  if (bar () != -13055525270329736316393717310914023773847wb)
    __builtin_abort ();
#endif
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BInt<156, 3, 24> = bar();
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BInt<156, 3, 24> = bitint::BInt::<156, 3, 24>::from_decimal_str(
// LOWERING-DAG:             "-13055525270329736316393717310914023773847",
// LOWERING-DAG:         );
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<156, 3, 24> = bar();
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<156, 3, 24> =
// REWRITES-DAG:         bitint::BInt::<156, 3, 24>::from_decimal_str("-13055525270329736316393717310914023773847");
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
