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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn classify({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut c: i32 = 0;
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     c = {{arg[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = c;
// LOWERING-DAG:         {
// LOWERING-DAG:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-DAG:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG:                 48 => 0,
// LOWERING-DAG:                 49 => 1,
// LOWERING-DAG:                 50 => 2,
// LOWERING-DAG:                 51 => 3,
// LOWERING-DAG:                 52 => 4,
// LOWERING-DAG:                 53 => 5,
// LOWERING-DAG:                 54 => 6,
// LOWERING-DAG:                 55 => 7,
// LOWERING-DAG:                 56 => 8,
// LOWERING-DAG:                 57 => 9,
// LOWERING-DAG:                 97 => 10,
// LOWERING-DAG:                 98 => 11,
// LOWERING-DAG:                 99 => 12,
// LOWERING-DAG:                 100 => 13,
// LOWERING-DAG:                 200 => 14,
// LOWERING-DAG:                 -3 => 15,
// LOWERING-DAG:                 -2 => 16,
// LOWERING-DAG:                 -1 => 17,
// LOWERING-DAG:                 7 => 18,
// LOWERING-DAG:                 _ => 19,
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
// LOWERING-DAG:                         __switch_case0 = 3;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     3 => {
// LOWERING-DAG:                         __switch_case0 = 4;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     4 => {
// LOWERING-DAG:                         __switch_case0 = 5;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     5 => {
// LOWERING-DAG:                         __switch_case0 = 6;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     6 => {
// LOWERING-DAG:                         __switch_case0 = 7;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     7 => {
// LOWERING-DAG:                         __switch_case0 = 8;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     8 => {
// LOWERING-DAG:                         __switch_case0 = 9;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     9 => {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         __retval = {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{_v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     10 => {
// LOWERING-DAG:                         __switch_case0 = 11;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     11 => {
// LOWERING-DAG:                         __switch_case0 = 12;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     12 => {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                         __retval = {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{_v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     13 => {
// LOWERING-DAG:                         __switch_case0 = 14;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     14 => {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                         __retval = {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{_v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     15 => {
// LOWERING-DAG:                         __switch_case0 = 16;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     16 => {
// LOWERING-DAG:                         __switch_case0 = 17;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     17 => {
// LOWERING-DAG:                         __switch_case0 = 18;
// LOWERING-DAG:                         continue '__switch0;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     18 => {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 4;
// LOWERING-DAG:                         __retval = {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                         return {{_v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                     19 => {
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
// REWRITES-DAG: fn classify(mut {{_v[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     match {{_v[0-9]+}} {
// REWRITES-DAG:         48..=57 => {
// REWRITES-DAG:             return 1;
// REWRITES-DAG:         }
// REWRITES-DAG:         97..=99 => {
// REWRITES-DAG:             return 2;
// REWRITES-DAG:         }
// REWRITES-DAG:         100 | 200 => {
// REWRITES-DAG:             return 3;
// REWRITES-DAG:         }
// REWRITES-DAG:         -3..=-1 | 7 => {
// REWRITES-DAG:             return 4;
// REWRITES-DAG:         }
// REWRITES-DAG:         _ => {
// REWRITES-DAG:             return 0;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     __retval
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
