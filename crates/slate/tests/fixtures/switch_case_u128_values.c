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
  case 340282366920938463463374607431768211450uwb ...
      340282366920938463463374607431768211453uwb:
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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn classify({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut v: u128 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: u128 = 340282366920938463463374607431768211440u128;
// LOWERING-DAG:     let {{_v[0-9]+}}: u128 = {{arg[0-9]+}} as u128;
// LOWERING-DAG:     let {{_v[0-9]+}}: u128 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     v = {{_v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: u128 = v;
// LOWERING-DAG:         {
// LOWERING-DAG:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-DAG:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG:                 340282366920938463463374607431768211441 => 0,
// LOWERING-DAG:                 340282366920938463463374607431768211443 => 1,
// LOWERING-DAG:                 340282366920938463463374607431768211450
// LOWERING-DAG:                     ..=340282366920938463463374607431768211453 => 2,
// LOWERING-DAG:                 _ => 3,
// LOWERING-DAG:             };
// LOWERING-DAG:             '__switch0: loop {
// LOWERING-DAG:                 match __switch_case0 {
// LOWERING-DAG:                     0 => {
// LOWERING-DAG:                         __switch_case0 = 1;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     1 => {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         __retval = {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{_v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     2 => {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                         __retval = {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{_v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     3 => {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         __retval = {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{_v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     _ => {
// LOWERING-DAG:                         break '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn classify_run({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut v: u128 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: u128 = 340282366920938463463374607431768211440u128;
// LOWERING-DAG:     let {{_v[0-9]+}}: u128 = {{arg[0-9]+}} as u128;
// LOWERING-DAG:     let {{_v[0-9]+}}: u128 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     v = {{_v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: u128 = v;
// LOWERING-DAG:         {
// LOWERING-DAG:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-DAG:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG:                 340282366920938463463374607431768211441 => 0,
// LOWERING-DAG:                 340282366920938463463374607431768211442 => 1,
// LOWERING-DAG:                 340282366920938463463374607431768211443 => 2,
// LOWERING-DAG:                 340282366920938463463374607431768211450 => 3,
// LOWERING-DAG:                 _ => 4,
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
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         __retval = {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{_v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     3 => {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                         __retval = {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{_v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     4 => {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         __retval = {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{_v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     _ => {
// LOWERING-DAG:                         break '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn classify({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     let mut v: u128 = 340282366920938463463374607431768211440u128 + ({{arg[0-9]+}} as u128);
// REWRITES-DAG:     match v {
// REWRITES-DAG:         340282366920938463463374607431768211441 | 340282366920938463463374607431768211443 => {
// REWRITES-DAG:             return 1;
// REWRITES-DAG:         }
// REWRITES-DAG:         340282366920938463463374607431768211450..=340282366920938463463374607431768211453 => {
// REWRITES-DAG:             return 2;
// REWRITES-DAG:         }
// REWRITES-DAG:         _ => {
// REWRITES-DAG:             return 0;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     __retval
// REWRITES-DAG: }
// REWRITES-DAG: fn classify_run({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     let mut v: u128 = 340282366920938463463374607431768211440u128 + ({{arg[0-9]+}} as u128);
// REWRITES-DAG:     match v {
// REWRITES-DAG:         340282366920938463463374607431768211441..=340282366920938463463374607431768211443 => {
// REWRITES-DAG:             return 1;
// REWRITES-DAG:         }
// REWRITES-DAG:         340282366920938463463374607431768211450 => {
// REWRITES-DAG:             return 2;
// REWRITES-DAG:         }
// REWRITES-DAG:         _ => {
// REWRITES-DAG:             return 0;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     __retval
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
