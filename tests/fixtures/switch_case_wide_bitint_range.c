#include <stdio.h>

typedef _BitInt(256) big;

// @lowering-fn-begin
// @rewrite-fn-begin
int classify(int seed) {
  big s = 170141183460469231731687303715884105727wb + (big)seed;
  switch (s) {
  case 170141183460469231731687303715884105727wb ... 170141183460469231731687303715884105730wb:
    return 1;
  case 170141183460469231731687303715884105740wb:
  case 170141183460469231731687303715884105745wb ... 170141183460469231731687303715884105747wb:
    return 2;
  default:
    return 0;
  }
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int probe(int seed) {
  big s = 170141183460469231731687303715884105727wb + (big)seed;
  int v = 42;
  switch (s) {
  case 170141183460469231731687303715884105727wb ... 170141183460469231731687303715884105730wb:
    return v;
  default:
    return 0;
  }
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d %d %d %d %d %d\n", classify(0), classify(13), classify(19),
         classify(5), probe(2), probe(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn classify({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut s: aligned::Aligned<aligned::A8, bitint::BInt<256, 4, 32>> =
// LOWERING-DAG:         aligned::Aligned(bitint::BInt::<256, 4, 32>::ZERO);
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> =
// LOWERING-DAG:         bitint::BInt::<256, 4, 32>::from_decimal_str("170141183460469231731687303715884105727");
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_i128({{arg[0-9]+}} as i128);
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     *s = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = *s;
// LOWERING-DAG:         {
// LOWERING-DAG:             let __switch_value0 = {{__v[0-9]+}};
// LOWERING-DAG:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG:                 __switch_sel
// LOWERING-DAG:                     if __switch_sel
// LOWERING-DAG:                         >= const {
// LOWERING-DAG:                             bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                 "170141183460469231731687303715884105727",
// LOWERING-DAG:                             )
// LOWERING-DAG:                         }
// LOWERING-DAG:                         && __switch_sel
// LOWERING-DAG:                             <= const {
// LOWERING-DAG:                                 bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                     "170141183460469231731687303715884105730",
// LOWERING-DAG:                                 )
// LOWERING-DAG:                             } =>
// LOWERING-DAG:                 {
// LOWERING-DAG:                     0
// LOWERING-DAG:                 }
// LOWERING-DAG:                 __switch_sel
// LOWERING-DAG:                     if __switch_sel
// LOWERING-DAG:                         == const {
// LOWERING-DAG:                             bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                 "170141183460469231731687303715884105740",
// LOWERING-DAG:                             )
// LOWERING-DAG:                         } =>
// LOWERING-DAG:                 {
// LOWERING-DAG:                     1
// LOWERING-DAG:                 }
// LOWERING-DAG:                 __switch_sel
// LOWERING-DAG:                     if __switch_sel
// LOWERING-DAG:                         >= const {
// LOWERING-DAG:                             bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                 "170141183460469231731687303715884105745",
// LOWERING-DAG:                             )
// LOWERING-DAG:                         }
// LOWERING-DAG:                         && __switch_sel
// LOWERING-DAG:                             <= const {
// LOWERING-DAG:                                 bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                     "170141183460469231731687303715884105747",
// LOWERING-DAG:                                 )
// LOWERING-DAG:                             } =>
// LOWERING-DAG:                 {
// LOWERING-DAG:                     2
// LOWERING-DAG:                 }
// LOWERING-DAG:                 _ => 3,
// LOWERING-DAG:             };
// LOWERING-DAG:             '__switch0: loop {
// LOWERING-DAG:                 match __switch_case0 {
// LOWERING-DAG:                     0 => {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{__v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     1 => {
// LOWERING-DAG:                         __switch_case0 = 2;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     2 => {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{__v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     3 => {
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
// LOWERING-DAG: fn probe({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut s: aligned::Aligned<aligned::A8, bitint::BInt<256, 4, 32>> =
// LOWERING-DAG:         aligned::Aligned(bitint::BInt::<256, 4, 32>::ZERO);
// LOWERING-DAG:     let mut v: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> =
// LOWERING-DAG:         bitint::BInt::<256, 4, 32>::from_decimal_str("170141183460469231731687303715884105727");
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_i128({{arg[0-9]+}} as i128);
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     *s = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 42;
// LOWERING-DAG:     v = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = *s;
// LOWERING-DAG:         {
// LOWERING-DAG:             let __switch_value0 = {{__v[0-9]+}};
// LOWERING-DAG:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG:                 __switch_sel
// LOWERING-DAG:                     if __switch_sel
// LOWERING-DAG:                         >= const {
// LOWERING-DAG:                             bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                 "170141183460469231731687303715884105727",
// LOWERING-DAG:                             )
// LOWERING-DAG:                         }
// LOWERING-DAG:                         && __switch_sel
// LOWERING-DAG:                             <= const {
// LOWERING-DAG:                                 bitint::BInt::<256, 4, 32>::from_decimal_str(
// LOWERING-DAG:                                     "170141183460469231731687303715884105730",
// LOWERING-DAG:                                 )
// LOWERING-DAG:                             } =>
// LOWERING-DAG:                 {
// LOWERING-DAG:                     0
// LOWERING-DAG:                 }
// LOWERING-DAG:                 _ => 1,
// LOWERING-DAG:             };
// LOWERING-DAG:             '__switch0: loop {
// LOWERING-DAG:                 match __switch_case0 {
// LOWERING-DAG:                     0 => {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = v;
// LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{__v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     1 => {
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
// REWRITES-DAG:     let mut s: aligned::Aligned<aligned::A8, bitint::BInt<256, 4, 32>> =
// REWRITES-DAG:         aligned::Aligned(bitint::BInt::<256, 4, 32>::ZERO);
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> =
// REWRITES-DAG:         bitint::BInt::<256, 4, 32>::from_decimal_str("170141183460469231731687303715884105727");
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_i128({{arg[0-9]+}} as i128);
// REWRITES-DAG:     *s = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     match *s {
// REWRITES-DAG:         __switch_sel
// REWRITES-DAG:             if __switch_sel
// REWRITES-DAG:                 >= const {
// REWRITES-DAG:                     bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                         "170141183460469231731687303715884105727",
// REWRITES-DAG:                     )
// REWRITES-DAG:                 }
// REWRITES-DAG:                 && __switch_sel
// REWRITES-DAG:                     <= const {
// REWRITES-DAG:                         bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                             "170141183460469231731687303715884105730",
// REWRITES-DAG:                         )
// REWRITES-DAG:                     } =>
// REWRITES-DAG:         {
// REWRITES-DAG:             return 1;
// REWRITES-DAG:         }
// REWRITES-DAG:         __switch_sel
// REWRITES-DAG:             if __switch_sel
// REWRITES-DAG:                 == const {
// REWRITES-DAG:                     bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                         "170141183460469231731687303715884105740",
// REWRITES-DAG:                     )
// REWRITES-DAG:                 }
// REWRITES-DAG:                 || __switch_sel
// REWRITES-DAG:                     >= const {
// REWRITES-DAG:                         bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                             "170141183460469231731687303715884105745",
// REWRITES-DAG:                         )
// REWRITES-DAG:                     }
// REWRITES-DAG:                     && __switch_sel
// REWRITES-DAG:                         <= const {
// REWRITES-DAG:                             bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                                 "170141183460469231731687303715884105747",
// REWRITES-DAG:                             )
// REWRITES-DAG:                         } =>
// REWRITES-DAG:         {
// REWRITES-DAG:             return 2;
// REWRITES-DAG:         }
// REWRITES-DAG:         _ => {
// REWRITES-DAG:             return 0;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     __retval
// REWRITES-DAG: }
// REWRITES-DAG: fn probe({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     let mut s: aligned::Aligned<aligned::A8, bitint::BInt<256, 4, 32>> =
// REWRITES-DAG:         aligned::Aligned(bitint::BInt::<256, 4, 32>::ZERO);
// REWRITES-DAG:     let mut v: i32 = 42;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> =
// REWRITES-DAG:         bitint::BInt::<256, 4, 32>::from_decimal_str("170141183460469231731687303715884105727");
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_i128({{arg[0-9]+}} as i128);
// REWRITES-DAG:     *s = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     match *s {
// REWRITES-DAG:         __switch_sel
// REWRITES-DAG:             if __switch_sel
// REWRITES-DAG:                 >= const {
// REWRITES-DAG:                     bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                         "170141183460469231731687303715884105727",
// REWRITES-DAG:                     )
// REWRITES-DAG:                 }
// REWRITES-DAG:                 && __switch_sel
// REWRITES-DAG:                     <= const {
// REWRITES-DAG:                         bitint::BInt::<256, 4, 32>::from_decimal_str(
// REWRITES-DAG:                             "170141183460469231731687303715884105730",
// REWRITES-DAG:                         )
// REWRITES-DAG:                     } =>
// REWRITES-DAG:         {
// REWRITES-DAG:             return v;
// REWRITES-DAG:         }
// REWRITES-DAG:         _ => {
// REWRITES-DAG:             return 0;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     __retval
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
