#include <stdio.h>

int score(int x) {
  int out = 0;
  // @lowering-begin
  // @rewrite-begin
  switch (x) {
  case 1:
    out += 10;
  case 2:
    out += 20;
    break;
  case 3:
  case 4:
    out += 40;
    break;
  default:
    out += 90;
  }
  // @rewrite-end
  // @lowering-end
  return out;
}

int main(void) {
  printf("%d %d %d %d %d\n", score(1), score(2), score(3), score(4), score(8));
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-DAG:             1 => 0,
// COMMON-LOWERING-DAG:             2 => 1,
// COMMON-LOWERING-DAG:             3 => 2,
// COMMON-LOWERING-DAG:             4 => 3,
// COMMON-LOWERING-DAG:             _ => 4,
// COMMON-LOWERING-DAG:         };
// COMMON-LOWERING-DAG:         '__switch0: loop {
// COMMON-LOWERING-DAG:             match __switch_case0 {
// COMMON-LOWERING-DAG:                 0 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     __switch_case0 = 1;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 1 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 20;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     break '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 2 => {
// COMMON-LOWERING-DAG:                     __switch_case0 = 3;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 3 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 40;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     break '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 4 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 90;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     break '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 _ => {
// COMMON-LOWERING-DAG:                     break '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: match {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:     1 => {
// COMMON-REWRITES-DAG:         out += 10;
// COMMON-REWRITES-DAG:         let _v15: i32 = 20;
// COMMON-REWRITES-DAG:         let _v16: i32 = out;
// COMMON-REWRITES-DAG:         let _v17: i32 = _v16 + _v15;
// COMMON-REWRITES-DAG:         out = _v17;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     2 => {
// COMMON-REWRITES-DAG:         out += 20;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     3 | 4 => {
// COMMON-REWRITES-DAG:         out += 40;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     _ => {
// COMMON-REWRITES-DAG:         out += 90;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
