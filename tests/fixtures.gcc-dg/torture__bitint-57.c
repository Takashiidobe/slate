/* PR tree-optimization/113774 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __BITINT_MAXWIDTH__ >= 512
unsigned _BitInt(512) u;
unsigned _BitInt(512) v;

// @lowering-fn-begin
// @rewrite-fn-begin
void
foo (unsigned _BitInt(255) a, unsigned _BitInt(257) b, unsigned _BitInt(512) *r)
{
  b += v;
  b |= a - b;
  unsigned _BitInt(512) c = b * 6;
  unsigned _BitInt(512) h = c >> u;
  *r = h;
}
// @rewrite-fn-end
// @lowering-fn-end
#endif

// @lowering-fn-begin
// @rewrite-fn-begin
int
main ()
{
#if __BITINT_MAXWIDTH__ >= 512
  unsigned _BitInt(512) x;
  foo (0x10000000000000000wb, 0x10000000000000001wb, &x);
  if (x != 0x1fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffawb)
    __builtin_abort ();
#endif
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: unsafe fn foo(
// LOWERING-DAG:     {{arg[0-9]+}}: bitint::BUint<255, 4, 32>,
// LOWERING-DAG:     {{arg[0-9]+}}: bitint::BUint<257, 5, 40>,
// LOWERING-DAG:     {{arg[0-9]+}}: *mut bitint::BUint<512, 8, 64>,
// LOWERING-DAG: ) {
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = unsafe { *v };
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_buint({{arg[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_buint({{__v[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_buint({{arg[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_decimal_str("6");
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_buint({{__v[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = unsafe { *u };
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     return;
// LOWERING-DAG: }
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut x: aligned::Aligned<aligned::A8, bitint::BUint<512, 8, 64>> =
// LOWERING-DAG:         aligned::Aligned(bitint::BUint::<512, 8, 64>::ZERO);
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<255, 4, 32> =
// LOWERING-DAG:         bitint::BUint::<255, 4, 32>::from_decimal_str("18446744073709551616");
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> =
// LOWERING-DAG:         bitint::BUint::<257, 5, 40>::from_decimal_str("18446744073709551617");
// LOWERING-DAG:     unsafe { foo({{__v[0-9]+}}, {{__v[0-9]+}}, std::ptr::addr_of_mut!(*x)) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = *x;
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_decimal_str(
// LOWERING-DAG:             "231584178474632390847141970017375815706539969331281128078915168015826259279866",
// LOWERING-DAG:         );
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe fn foo(
// REWRITES-DAG:     {{arg[0-9]+}}: bitint::BUint<255, 4, 32>,
// REWRITES-DAG:     {{arg[0-9]+}}: bitint::BUint<257, 5, 40>,
// REWRITES-DAG:     {{arg[0-9]+}}: *mut bitint::BUint<512, 8, 64>,
// REWRITES-DAG: ) {
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = unsafe { *v };
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_buint({{arg[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_buint({{__v[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_buint({{arg[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = {{__v[0-9]+}} - {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = {{__v[0-9]+}} | {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_decimal_str("6");
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = {{__v[0-9]+}} * {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_buint({{__v[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = unsafe { *u };
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     return;
// REWRITES-DAG: }
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut x: aligned::Aligned<aligned::A8, bitint::BUint<512, 8, 64>> =
// REWRITES-DAG:         aligned::Aligned(bitint::BUint::<512, 8, 64>::ZERO);
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<255, 4, 32> =
// REWRITES-DAG:         bitint::BUint::<255, 4, 32>::from_decimal_str("18446744073709551616");
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> =
// REWRITES-DAG:         bitint::BUint::<257, 5, 40>::from_decimal_str("18446744073709551617");
// REWRITES-DAG:     unsafe { foo({{__v[0-9]+}}, {{__v[0-9]+}}, std::ptr::addr_of_mut!(*x)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = *x;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_decimal_str(
// REWRITES-DAG:         "231584178474632390847141970017375815706539969331281128078915168015826259279866",
// REWRITES-DAG:     );
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
