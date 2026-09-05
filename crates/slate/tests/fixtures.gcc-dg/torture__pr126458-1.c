/* { dg-do run { target bitint } } */
/* PR tree-optimization/126458 */

typedef unsigned _BitInt(17) u17;
typedef unsigned _BitInt(16) u16;
typedef unsigned _BitInt(15) u15;
typedef signed _BitInt(16) s16;

// @lowering-fn-begin
// @rewrite-fn-begin
__attribute__((noipa)) int neg(u16 a) { return ((s16)a) < 0; }
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
__attribute__((noipa)) u17 fref(u16 a) {
  return neg(a) ? (u17)a : (u17)(u16)-1u;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
__attribute__((noipa)) u17 f(u16 a) {
  return ((s16)a) < 0 ? (u17)a : (u17)(u16)-1u;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  static const u16 v[] = {((u16)1u) << 15, (u16)-2u, (u16)-1u, (u16)(u15)-1u,
                          0u};

  for (unsigned i = 0; i < sizeof v / sizeof v[0]; i++)
    if (f(v[i]) != fref(v[i]))
      __builtin_abort();
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn neg({{arg[0-9]+}}: bitint::BUint<16, 1, 2>) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<16, 1, 2> = bitint::BInt::<16, 1, 2>::from_buint({{arg[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn fref({{arg[0-9]+}}: bitint::BUint<16, 1, 2>) -> bitint::BUint<17, 1, 4> {
// LOWERING-DAG:     let mut a: aligned::Aligned<aligned::A2, bitint::BUint<16, 1, 2>> =
// LOWERING-DAG:         aligned::Aligned(bitint::BUint::<16, 1, 2>::ZERO);
// LOWERING-DAG:     *a = {{arg[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<16, 1, 2> = *a;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = neg({{__v[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = if {{__v[0-9]+}} {
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BUint<16, 1, 2> = *a;
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_buint({{__v[0-9]+}});
// LOWERING-DAG:         {{__v[0-9]+}}
// LOWERING-DAG:     } else {
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_decimal_str("65535");
// LOWERING-DAG:         {{__v[0-9]+}}
// LOWERING-DAG:     };
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn f({{arg[0-9]+}}: bitint::BUint<16, 1, 2>) -> bitint::BUint<17, 1, 4> {
// LOWERING-DAG:     let mut a: aligned::Aligned<aligned::A2, bitint::BUint<16, 1, 2>> =
// LOWERING-DAG:         aligned::Aligned(bitint::BUint::<16, 1, 2>::ZERO);
// LOWERING-DAG:     *a = {{arg[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<16, 1, 2> = *a;
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<16, 1, 2> = bitint::BInt::<16, 1, 2>::from_buint({{__v[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = if {{__v[0-9]+}} {
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BUint<16, 1, 2> = *a;
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_buint({{__v[0-9]+}});
// LOWERING-DAG:         {{__v[0-9]+}}
// LOWERING-DAG:     } else {
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_decimal_str("65535");
// LOWERING-DAG:         {{__v[0-9]+}}
// LOWERING-DAG:     };
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: u32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: u32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: u32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-DAG:             let {{__v[0-9]+}}: u64 = 10;
// LOWERING-DAG:             let {{__v[0-9]+}}: u64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let {{__v[0-9]+}}: u32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-DAG:                 let {{__v[0-9]+}}: bitint::BUint<16, 1, 2> = unsafe { (*main_v)[({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:                 let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = f({{__v[0-9]+}});
// LOWERING-DAG:                 let {{__v[0-9]+}}: u32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-DAG:                 let {{__v[0-9]+}}: bitint::BUint<16, 1, 2> = unsafe { (*main_v)[({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:                 let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = fref({{__v[0-9]+}});
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                 if {{__v[0-9]+}} {
// LOWERING-DAG:                     unsafe { abort() };
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: u32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn neg({{arg[0-9]+}}: bitint::BUint<16, 1, 2>) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<16, 1, 2> = bitint::BInt::<16, 1, 2>::from_buint({{arg[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// REWRITES-DAG:     ({{__v[0-9]+}} < 0) as i32
// REWRITES-DAG: }
// REWRITES-DAG: fn fref({{arg[0-9]+}}: bitint::BUint<16, 1, 2>) -> bitint::BUint<17, 1, 4> {
// REWRITES-DAG:     let mut a: aligned::Aligned<aligned::A2, bitint::BUint<16, 1, 2>> =
// REWRITES-DAG:         aligned::Aligned(bitint::BUint::<16, 1, 2>::ZERO);
// REWRITES-DAG:     *a = {{arg[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = neg(*a);
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_buint(*a);
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_decimal_str("65535");
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     return {{__v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: fn f({{arg[0-9]+}}: bitint::BUint<16, 1, 2>) -> bitint::BUint<17, 1, 4> {
// REWRITES-DAG:     let mut a: aligned::Aligned<aligned::A2, bitint::BUint<16, 1, 2>> =
// REWRITES-DAG:         aligned::Aligned(bitint::BUint::<16, 1, 2>::ZERO);
// REWRITES-DAG:     *a = {{arg[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<16, 1, 2> = bitint::BInt::<16, 1, 2>::from_buint(*a);
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_buint(*a);
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_decimal_str("65535");
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     return {{__v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut i: u32 = 0;
// REWRITES-DAG:     i = 0;
// REWRITES-DAG:     loop {
// REWRITES-DAG:         let {{__v[0-9]+}}: u64 = 2;
// REWRITES-DAG:         if !((i as u64) < 10 / {{__v[0-9]+}}) {
// REWRITES-DAG:             break;
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = f(unsafe { (*main_v)[((i as u64) as usize)] });
// REWRITES-DAG:         let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = fref(unsafe { (*main_v)[((i as u64) as usize)] });
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { std::process::abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
