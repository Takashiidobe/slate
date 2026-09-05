#include <stdio.h>

typedef unsigned _BitInt(128) u128;

// @lowering-fn-begin
// @rewrite-fn-begin
int classify(int seed) {
  u128 v = (u128)340282366920938463463374607431768211440uwb + (u128)seed;
  switch (v) {
  case 340282366920938463463374607431768211441uwb:
  case 340282366920938463463374607431768211443uwb:
    return 1;
  case 340282366920938463463374607431768211450uwb ... 340282366920938463463374607431768211453uwb:
    return 2;
  default:
    return 0;
  }
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int classify_run(int seed) {
  u128 v = (u128)340282366920938463463374607431768211440uwb + (u128)seed;
  switch (v) {
  case 340282366920938463463374607431768211441uwb:
  case 340282366920938463463374607431768211442uwb:
  case 340282366920938463463374607431768211443uwb:
    return 1;
  case 340282366920938463463374607431768211450uwb:
    return 2;
  default:
    return 0;
  }
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d %d %d %d %d %d %d\n", classify(1), classify(3), classify(11),
         classify(5), classify_run(2), classify_run(10), classify_run(5));
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn classify({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     let mut v: u128 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u128 = 340282366920938463463374607431768211440u128;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u128 = {{arg[0-9]+}} as u128;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     v = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: u128 = v;
// COMMON-LOWERING-DAG:         {
// COMMON-LOWERING-DAG:             let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-DAG:                 340282366920938463463374607431768211441 => 0,
// COMMON-LOWERING-DAG:                 340282366920938463463374607431768211443 => 1,
// COMMON-LOWERING-DAG:                 340282366920938463463374607431768211450
// COMMON-LOWERING-DAG:                     ..=340282366920938463463374607431768211453 => 2,
// COMMON-LOWERING-DAG:                 _ => 3,
// COMMON-LOWERING-DAG:             };
// COMMON-LOWERING-DAG:             '__switch0: loop {
// COMMON-LOWERING-DAG:                 match __switch_case0 {
// COMMON-LOWERING-DAG:                     0 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 1;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     1 => {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:                         return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     2 => {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:                         return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     3 => {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:                         return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     _ => {
// COMMON-LOWERING-DAG:                         break '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn classify_run({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     let mut v: u128 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u128 = 340282366920938463463374607431768211440u128;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u128 = {{arg[0-9]+}} as u128;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     v = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: u128 = v;
// COMMON-LOWERING-DAG:         {
// COMMON-LOWERING-DAG:             let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-DAG:                 340282366920938463463374607431768211441 => 0,
// COMMON-LOWERING-DAG:                 340282366920938463463374607431768211442 => 1,
// COMMON-LOWERING-DAG:                 340282366920938463463374607431768211443 => 2,
// COMMON-LOWERING-DAG:                 340282366920938463463374607431768211450 => 3,
// COMMON-LOWERING-DAG:                 _ => 4,
// COMMON-LOWERING-DAG:             };
// COMMON-LOWERING-DAG:             '__switch0: loop {
// COMMON-LOWERING-DAG:                 match __switch_case0 {
// COMMON-LOWERING-DAG:                     0 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 1;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     1 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 2;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     2 => {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:                         return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     3 => {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:                         return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     4 => {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:                         return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     _ => {
// COMMON-LOWERING-DAG:                         break '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn classify({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-DAG:     let mut __retval: i32 = 0;
// COMMON-REWRITES-DAG:     let mut v: u128 = 340282366920938463463374607431768211440u128 + ({{arg[0-9]+}} as u128);
// COMMON-REWRITES-DAG:     match v {
// COMMON-REWRITES-DAG:         340282366920938463463374607431768211441 | 340282366920938463463374607431768211443 => {
// COMMON-REWRITES-DAG:             return 1;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         340282366920938463463374607431768211450..=340282366920938463463374607431768211453 => {
// COMMON-REWRITES-DAG:             return 2;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         _ => {
// COMMON-REWRITES-DAG:             return 0;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     __retval
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn classify_run({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-DAG:     let mut __retval: i32 = 0;
// COMMON-REWRITES-DAG:     let mut v: u128 = 340282366920938463463374607431768211440u128 + ({{arg[0-9]+}} as u128);
// COMMON-REWRITES-DAG:     match v {
// COMMON-REWRITES-DAG:         340282366920938463463374607431768211441..=340282366920938463463374607431768211443 => {
// COMMON-REWRITES-DAG:             return 1;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         340282366920938463463374607431768211450 => {
// COMMON-REWRITES-DAG:             return 2;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         _ => {
// COMMON-REWRITES-DAG:             return 0;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     __retval
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
