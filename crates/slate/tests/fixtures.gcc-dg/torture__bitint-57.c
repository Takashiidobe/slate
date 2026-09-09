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
void foo(unsigned _BitInt(255) a, unsigned _BitInt(257) b,
         unsigned _BitInt(512) * r) {
  b                       += v;
  b                       |= a - b;
  unsigned _BitInt(512) c  = b * 6;
  unsigned _BitInt(512) h  = c >> u;
  *r                       = h;
}
// @rewrite-fn-end
// @lowering-fn-end
#endif

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
#if __BITINT_MAXWIDTH__ >= 512
  unsigned _BitInt(512) x;
  foo(0x10000000000000000wb, 0x10000000000000001wb, &x);
  if (x !=
      0x1fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffawb)
    __builtin_abort();
#endif
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe fn foo(
// REWRITES-DAG:     {{arg[0-9]+}}: bitint::BUint<255, 4, 32>,
// REWRITES-DAG:     {{arg[0-9]+}}: bitint::BUint<257, 5, 40>,
// REWRITES-DAG:     {{arg[0-9]+}}: *mut bitint::BUint<512, 8, 64>,
// REWRITES-DAG: ) {
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = unsafe { *v };
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_buint({{arg[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_buint({{__v[0-9]+}} + {{__v[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_buint({{arg[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = {{__v[0-9]+}} | {{__v[0-9]+}} - {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_decimal_str("6");
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_buint({{__v[0-9]+}} * {{__v[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = unsafe { *u };
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = {{__v[0-9]+}} >> {{__v[0-9]+}}.to_u128();
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     return;
// REWRITES-DAG: }
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     let mut x: aligned::Aligned<aligned::A8, bitint::BUint<512, 8, 64>> =
// REWRITES-DAG:         aligned::Aligned(bitint::BUint::<512, 8, 64>::ZERO);
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         foo(
// REWRITES-DAG:             bitint::BUint::<255, 4, 32>::from_decimal_str("18446744073709551616"),
// REWRITES-DAG:             bitint::BUint::<257, 5, 40>::from_decimal_str("18446744073709551617"),
// REWRITES-DAG:             std::ptr::addr_of_mut!(*x),
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = *x;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_decimal_str(
// REWRITES-DAG:         "231584178474632390847141970017375815706539969331281128078915168015826259279866",
// REWRITES-DAG:     );
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: unsafe fn foo(
// LOWERING-X86_64-GNU-DAG:     {{arg[0-9]+}}: bitint::BUint<255, 4, 32>,
// LOWERING-X86_64-GNU-DAG:     {{arg[0-9]+}}: bitint::BUint<257, 5, 40>,
// LOWERING-X86_64-GNU-DAG:     {{arg[0-9]+}}: *mut bitint::BUint<512, 8, 64>,
// LOWERING-X86_64-GNU-DAG: ) {
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = unsafe { *v };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_buint({{arg[0-9]+}});
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_buint({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_buint({{arg[0-9]+}});
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = bitint::BUint::<257, 5, 40>::from_decimal_str("6");
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_buint({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = unsafe { *u };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = {{__v[0-9]+}} >> {{__v[0-9]+}}.to_u128();
// LOWERING-X86_64-GNU-DAG:     unsafe {
// LOWERING-X86_64-GNU-DAG:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     }
// LOWERING-X86_64-GNU-DAG:     return;
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: fn main() -> std::process::ExitCode {
// LOWERING-X86_64-GNU-DAG:     let mut x: aligned::Aligned<aligned::A8, bitint::BUint<512, 8, 64>> =
// LOWERING-X86_64-GNU-DAG:         aligned::Aligned(bitint::BUint::<512, 8, 64>::ZERO);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<255, 4, 32> =
// LOWERING-X86_64-GNU-DAG:         bitint::BUint::<255, 4, 32>::from_decimal_str("18446744073709551616");
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BUint<257, 5, 40> =
// LOWERING-X86_64-GNU-DAG:         bitint::BUint::<257, 5, 40>::from_decimal_str("18446744073709551617");
// LOWERING-X86_64-GNU-DAG:     unsafe { foo({{__v[0-9]+}}, {{__v[0-9]+}}, std::ptr::addr_of_mut!(*x)) };
// LOWERING-X86_64-GNU-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = *x;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bitint::BUint<512, 8, 64> = bitint::BUint::<512, 8, 64>::from_decimal_str(
// LOWERING-X86_64-GNU-DAG:             "231584178474632390847141970017375815706539969331281128078915168015826259279866",
// LOWERING-X86_64-GNU-DAG:         );
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:         if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-DAG:             unsafe { abort() };
// LOWERING-X86_64-GNU-DAG:         }
// LOWERING-X86_64-GNU-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering
