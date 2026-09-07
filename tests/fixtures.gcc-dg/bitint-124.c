/* PR tree-optimization/121131 */
/* { dg-do run { target bitint } } */
/* { dg-options "-O2" } */

#if __BITINT_MAXWIDTH__ >= 156
struct A {
  _BitInt(156) b : 135;
};

static inline _BitInt(156) foo(struct A *x) { return x[1].b; }

__attribute__((noipa)) _BitInt(156) bar(void) {
  struct A a[] = {
      1, 1, -13055525270329736316393717310914023773847wb, 1, 1, 1, 1, 1, 1, 1,
      1, 1};
  return foo(&a[1]);
}
#endif

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
#if __BITINT_MAXWIDTH__ >= 156
  if (bar() != -13055525270329736316393717310914023773847wb)
    __builtin_abort();
#endif
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bitint::BInt<156, 3, 24> = bar();
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bitint::BInt<156, 3, 24> = bitint::BInt::<156, 3, 24>::from_decimal_str(
// LOWERING-X86_64-GNU-DAG:             "-13055525270329736316393717310914023773847",
// LOWERING-X86_64-GNU-DAG:         );
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:         if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-DAG:             unsafe { abort() };
// LOWERING-X86_64-GNU-DAG:         }
// LOWERING-X86_64-GNU-DAG:     }
// LOWERING-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BInt<156, 3, 24> = bar();
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bitint::BInt<156, 3, 24> =
// REWRITES-X86_64-GNU-DAG:         bitint::BInt::<156, 3, 24>::from_decimal_str("-13055525270329736316393717310914023773847");
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-X86_64-GNU-DAG:     if {{__v[0-9]+}} {
// REWRITES-X86_64-GNU-DAG:         unsafe { std::process::abort() };
// REWRITES-X86_64-GNU-DAG:     }
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
