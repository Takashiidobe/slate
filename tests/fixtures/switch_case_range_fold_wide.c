#include <stdio.h>

typedef _BitInt(128) big;

// @lowering-fn-begin
// @rewrite-fn-begin
int classify_u64(unsigned long long v) {
  switch (v) {
  case 18446744073709551610ULL:
  case 18446744073709551611ULL:
  case 18446744073709551612ULL:
    return 1;
  case 18446744073709551615ULL:
    return 2;
  default:
    return 0;
  }
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int classify_bitint(int seed) {
  big v = (big)170141183460469231731687303715884105720wb + (big)seed;
  switch (v) {
  case 170141183460469231731687303715884105720wb:
  case 170141183460469231731687303715884105721wb:
  case 170141183460469231731687303715884105722wb:
    return 1;
  case 170141183460469231731687303715884105727wb:
    return 2;
  default:
    return 0;
  }
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d %d %d %d %d %d\n", classify_u64(18446744073709551611ULL),
         classify_u64(18446744073709551615ULL), classify_u64(3),
         classify_bitint(1), classify_bitint(7), classify_bitint(4));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn classify_u64({{arg[0-9]+}}: u64) -> i32 {
// LOWERING-DAG:     let mut v: u64 = 0;
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     v = {{arg[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: u64 = v;
// LOWERING-DAG:         {
// LOWERING-DAG:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-DAG:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG:                 18446744073709551610 => 0,
// LOWERING-DAG:                 18446744073709551611 => 1,
// LOWERING-DAG:                 18446744073709551612 => 2,
// LOWERING-DAG:                 18446744073709551615 => 3,
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
// LOWERING-DAG: fn classify_bitint({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut v: i128 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: i128 = 170141183460469231731687303715884105720i128;
// LOWERING-DAG:     let {{_v[0-9]+}}: i128 = {{arg[0-9]+}} as i128;
// LOWERING-DAG:     let {{_v[0-9]+}}: i128 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     v = {{_v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i128 = v;
// LOWERING-DAG:         {
// LOWERING-DAG:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-DAG:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG:                 170141183460469231731687303715884105720 => 0,
// LOWERING-DAG:                 170141183460469231731687303715884105721 => 1,
// LOWERING-DAG:                 170141183460469231731687303715884105722 => 2,
// LOWERING-DAG:                 170141183460469231731687303715884105727 => 3,
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
// REWRITES-DAG: fn classify_u64({{arg[0-9]+}}: u64) -> i32 {
// REWRITES-DAG:     let mut v: u64 = {{arg[0-9]+}};
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     match v {
// REWRITES-DAG:         18446744073709551610..=18446744073709551612 => {
// REWRITES-DAG:             __retval = 1;
// REWRITES-DAG:             return __retval;
// REWRITES-DAG:         }
// REWRITES-DAG:         18446744073709551615 => {
// REWRITES-DAG:             __retval = 2;
// REWRITES-DAG:             return __retval;
// REWRITES-DAG:         }
// REWRITES-DAG:         _ => {
// REWRITES-DAG:             __retval = 0;
// REWRITES-DAG:             return __retval;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     return __retval;
// REWRITES-DAG: }
// REWRITES-DAG: fn classify_bitint({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     let mut v: i128 = 170141183460469231731687303715884105720i128 + ({{arg[0-9]+}} as i128);
// REWRITES-DAG:     match v {
// REWRITES-DAG:         170141183460469231731687303715884105720..=170141183460469231731687303715884105722 => {
// REWRITES-DAG:             __retval = 1;
// REWRITES-DAG:             return __retval;
// REWRITES-DAG:         }
// REWRITES-DAG:         170141183460469231731687303715884105727 => {
// REWRITES-DAG:             __retval = 2;
// REWRITES-DAG:             return __retval;
// REWRITES-DAG:         }
// REWRITES-DAG:         _ => {
// REWRITES-DAG:             __retval = 0;
// REWRITES-DAG:             return __retval;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
