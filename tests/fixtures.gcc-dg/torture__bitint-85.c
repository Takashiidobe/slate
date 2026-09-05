/* { dg-do run { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __BITINT_MAXWIDTH__ >= 1024
constexpr _BitInt(1024) d = -541140097068598424394740839221562143161511518875518765552323978870598341733206554363735813878577506997168480201818027232521wb;
int c;

static inline void
foo (_BitInt(1024) b, _BitInt(1024) *r)
{
  if (c)
    b = 0;
  *r = b;
}

// @lowering-fn-begin
// @rewrite-fn-begin
[[gnu::noipa]] void
bar (_BitInt(1024) y)
{
  if (y != d)
    __builtin_abort ();
}
// @rewrite-fn-end
// @lowering-fn-end
#endif

// @lowering-fn-begin
// @rewrite-fn-begin
int
main ()
{
#if __BITINT_MAXWIDTH__ >= 1024
  _BitInt(1024) x;
  foo (d, &x);
  bar (x);
#endif
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn bar({{arg[0-9]+}}: bitint::BInt<1024, 16, 128>) {
// LOWERING-DAG:     let mut y: aligned::Aligned<aligned::A8, bitint::BInt<1024, 16, 128>> =
// LOWERING-DAG:         aligned::Aligned(bitint::BInt::<1024, 16, 128>::ZERO);
// LOWERING-DAG:     *y = {{arg[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BInt<1024, 16, 128> = *y;
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BInt<1024, 16, 128> = unsafe { *d };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     return;
// LOWERING-DAG: }
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut x: aligned::Aligned<aligned::A8, bitint::BInt<1024, 16, 128>> =
// LOWERING-DAG:         aligned::Aligned(bitint::BInt::<1024, 16, 128>::ZERO);
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<1024, 16, 128> = unsafe { *d };
// LOWERING-DAG:     foo({{__v[0-9]+}}, std::ptr::addr_of_mut!(*x));
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<1024, 16, 128> = *x;
// LOWERING-DAG:     bar({{__v[0-9]+}});
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn bar({{arg[0-9]+}}: bitint::BInt<1024, 16, 128>) {
// REWRITES-DAG:     let mut y: aligned::Aligned<aligned::A8, bitint::BInt<1024, 16, 128>> =
// REWRITES-DAG:         aligned::Aligned(bitint::BInt::<1024, 16, 128>::ZERO);
// REWRITES-DAG:     *y = {{arg[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<1024, 16, 128> = *y;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<1024, 16, 128> = unsafe { *d };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     return;
// REWRITES-DAG: }
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut x: aligned::Aligned<aligned::A8, bitint::BInt<1024, 16, 128>> =
// REWRITES-DAG:         aligned::Aligned(bitint::BInt::<1024, 16, 128>::ZERO);
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<1024, 16, 128> = unsafe { *d };
// REWRITES-DAG:     foo({{__v[0-9]+}}, unsafe { &mut (*std::ptr::addr_of_mut!(*x)) });
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<1024, 16, 128> = *x;
// REWRITES-DAG:     bar({{__v[0-9]+}});
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
