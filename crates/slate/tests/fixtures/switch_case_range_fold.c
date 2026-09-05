#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
int classify(int c) {
  switch (c) {
  case 48:
  case 49:
  case 50:
  case 51:
  case 52:
  case 53:
  case 54:
  case 55:
  case 56:
  case 57:
    return 1;
  case 'a':
  case 'b':
  case 'c':
    return 2;
  case 100:
  case 200:
    return 3;
  case -3:
  case -2:
  case -1:
  case 7:
    return 4;
  default:
    return 0;
  }
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d %d %d %d %d %d %d\n", classify('5'), classify('b'), classify(100),
         classify(200), classify(-2), classify(7), classify(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn classify({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut c: i32 = 0;
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     c = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = c;
// COMMON-LOWERING-DAG:         {
// COMMON-LOWERING-DAG:             let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-DAG:                 48 => 0,
// COMMON-LOWERING-DAG:                 49 => 1,
// COMMON-LOWERING-DAG:                 50 => 2,
// COMMON-LOWERING-DAG:                 51 => 3,
// COMMON-LOWERING-DAG:                 52 => 4,
// COMMON-LOWERING-DAG:                 53 => 5,
// COMMON-LOWERING-DAG:                 54 => 6,
// COMMON-LOWERING-DAG:                 55 => 7,
// COMMON-LOWERING-DAG:                 56 => 8,
// COMMON-LOWERING-DAG:                 57 => 9,
// COMMON-LOWERING-DAG:                 97 => 10,
// COMMON-LOWERING-DAG:                 98 => 11,
// COMMON-LOWERING-DAG:                 99 => 12,
// COMMON-LOWERING-DAG:                 100 => 13,
// COMMON-LOWERING-DAG:                 200 => 14,
// COMMON-LOWERING-DAG:                 -3 => 15,
// COMMON-LOWERING-DAG:                 -2 => 16,
// COMMON-LOWERING-DAG:                 -1 => 17,
// COMMON-LOWERING-DAG:                 7 => 18,
// COMMON-LOWERING-DAG:                 _ => 19,
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
// COMMON-LOWERING-DAG:                         __switch_case0 = 3;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     3 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 4;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     4 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 5;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     5 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 6;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     6 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 7;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     7 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 8;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     8 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 9;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     9 => {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:                         return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     10 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 11;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     11 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 12;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     12 => {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:                         return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     13 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 14;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     14 => {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:                         return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     15 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 16;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     16 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 17;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     17 => {
// COMMON-LOWERING-DAG:                         __switch_case0 = 18;
// COMMON-LOWERING-DAG:                         continue '__switch0;
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     18 => {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-DAG:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:                         return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     19 => {
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
// COMMON-REWRITES-DAG: fn classify(mut {{__v[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-DAG:     let mut __retval: i32 = 0;
// COMMON-REWRITES-DAG:     match {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         48..=57 => {
// COMMON-REWRITES-DAG:             return 1;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         97..=99 => {
// COMMON-REWRITES-DAG:             return 2;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         100 | 200 => {
// COMMON-REWRITES-DAG:             return 3;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         -3..=-1 | 7 => {
// COMMON-REWRITES-DAG:             return 4;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         _ => {
// COMMON-REWRITES-DAG:             return 0;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     __retval
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
