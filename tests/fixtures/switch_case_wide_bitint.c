#include <stdio.h>

typedef _BitInt(256) big;

// @lowering-fn-begin
// @rewrite-fn-begin
int classify(int seed) {
  big v = 170141183460469231731687303715884105727wb + (big)seed;
  switch (v) {
  case 170141183460469231731687303715884105727wb:
  case 170141183460469231731687303715884105728wb:
  case 170141183460469231731687303715884105729wb:
    return 1;
  case 170141183460469231731687303715884105736wb:
    return 2;
  case -170141183460469231731687303715884105727wb:
    return 3;
  default:
    return 0;
  }
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d %d %d %d\n", classify(0), classify(2), classify(9), classify(5));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn classify({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut v: aligned::Aligned<aligned::A8, bitint::BInt<256, 4, 32>> =
// LOWERING-DAG:         aligned::Aligned(bitint::BInt::<256, 4, 32>::ZERO);
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> =
// LOWERING-DAG:         bitint::BInt::<256, 4, 32>::from_decimal_str("170141183460469231731687303715884105727");
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_i128({{arg[0-9]+}} as i128);
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     *v = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = *v;
// LOWERING-DAG:         {
// LOWERING-DAG:             let __switch_value0 = {{__v[0-9]+}};
// LOWERING-DAG:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG:                 __switch_sel
// LOWERING-DAG:                     if __switch_sel
// LOWERING-DAG:                         == const {
// LOWERING-DAG:                             bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                 "170141183460469231731687303715884105727",
// LOWERING-DAG:                             )
// LOWERING-DAG:                         } =>
// LOWERING-DAG:                 {
// LOWERING-DAG:                     0
// LOWERING-DAG:                 }
// LOWERING-DAG:                 __switch_sel
// LOWERING-DAG:                     if __switch_sel
// LOWERING-DAG:                         == const {
// LOWERING-DAG:                             bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                 "170141183460469231731687303715884105728",
// LOWERING-DAG:                             )
// LOWERING-DAG:                         } =>
// LOWERING-DAG:                 {
// LOWERING-DAG:                     1
// LOWERING-DAG:                 }
// LOWERING-DAG:                 __switch_sel
// LOWERING-DAG:                     if __switch_sel
// LOWERING-DAG:                         == const {
// LOWERING-DAG:                             bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                 "170141183460469231731687303715884105729",
// LOWERING-DAG:                             )
// LOWERING-DAG:                         } =>
// LOWERING-DAG:                 {
// LOWERING-DAG:                     2
// LOWERING-DAG:                 }
// LOWERING-DAG:                 __switch_sel
// LOWERING-DAG:                     if __switch_sel
// LOWERING-DAG:                         == const {
// LOWERING-DAG:                             bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                 "170141183460469231731687303715884105736",
// LOWERING-DAG:                             )
// LOWERING-DAG:                         } =>
// LOWERING-DAG:                 {
// LOWERING-DAG:                     3
// LOWERING-DAG:                 }
// LOWERING-DAG:                 __switch_sel
// LOWERING-DAG:                     if __switch_sel
// LOWERING-DAG:                         == const {
// LOWERING-DAG:                             bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                 "-170141183460469231731687303715884105727",
// LOWERING-DAG:                             )
// LOWERING-DAG:                         } =>
// LOWERING-DAG:                 {
// LOWERING-DAG:                     4
// LOWERING-DAG:                 }
// LOWERING-DAG:                 _ => 5,
// LOWERING-DAG:             };
// LOWERING-DAG:             '__switch0: loop {
// LOWERING-DAG:                 match __switch_case0 {
// LOWERING-DAG:                     0 => {
// LOWERING-DAG:                         __switch_case0 = 1;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     1 => {
// LOWERING-DAG:                         __switch_case0 = 2;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     2 => {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{__v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     3 => {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{__v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     4 => {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{__v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     5 => {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{__v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     _ => {
// LOWERING-DAG:                         break '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn classify({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     let mut v: aligned::Aligned<aligned::A8, bitint::BInt<256, 4, 32>> =
// REWRITES-DAG:         aligned::Aligned(bitint::BInt::<256, 4, 32>::ZERO);
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> =
// REWRITES-DAG:         bitint::BInt::<256, 4, 32>::from_decimal_str("170141183460469231731687303715884105727");
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_i128({{arg[0-9]+}} as i128);
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     *v = {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = *v;
// REWRITES-DAG:     match {{__v[0-9]+}} {
// REWRITES-DAG:         __switch_sel
// REWRITES-DAG:             if __switch_sel
// REWRITES-DAG:                 == const {
// REWRITES-DAG:                     bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                         "170141183460469231731687303715884105727",
// REWRITES-DAG:                     )
// REWRITES-DAG:                 }
// REWRITES-DAG:                 || __switch_sel
// REWRITES-DAG:                     == const {
// REWRITES-DAG:                         bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                             "170141183460469231731687303715884105728",
// REWRITES-DAG:                         )
// REWRITES-DAG:                     }
// REWRITES-DAG:                 || __switch_sel
// REWRITES-DAG:                     == const {
// REWRITES-DAG:                         bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                             "170141183460469231731687303715884105729",
// REWRITES-DAG:                         )
// REWRITES-DAG:                     } =>
// REWRITES-DAG:         {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:             __retval = {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// REWRITES-DAG:             return {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         __switch_sel
// REWRITES-DAG:             if __switch_sel
// REWRITES-DAG:                 == const {
// REWRITES-DAG:                     bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                         "170141183460469231731687303715884105736",
// REWRITES-DAG:                     )
// REWRITES-DAG:                 } =>
// REWRITES-DAG:         {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:             __retval = {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// REWRITES-DAG:             return {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         __switch_sel
// REWRITES-DAG:             if __switch_sel
// REWRITES-DAG:                 == const {
// REWRITES-DAG:                     bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                         "-170141183460469231731687303715884105727",
// REWRITES-DAG:                     )
// REWRITES-DAG:                 } =>
// REWRITES-DAG:         {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:             __retval = {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// REWRITES-DAG:             return {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         _ => {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             __retval = {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// REWRITES-DAG:             return {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
