#include <stdio.h>

int score(int x) {
  int out = 0;
  // @lowering-begin
  // @rewrite-begin
  switch (x) {
  default:
    out += 1;
  case 5:
    out += 10;
  case 6:
    out += 20;
    break;
  case 7:
    out += 40;
  }
  // @rewrite-end
  // @lowering-end
  return out;
}

int main(void) {
  printf("%d %d %d %d\n", score(5), score(6), score(7), score(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-DAG:             5 => 1,
// COMMON-LOWERING-DAG:             6 => 2,
// COMMON-LOWERING-DAG:             7 => 3,
// COMMON-LOWERING-DAG:             _ => 0,
// COMMON-LOWERING-DAG:         };
// COMMON-LOWERING-DAG:         '__switch0: loop {
// COMMON-LOWERING-DAG:             match __switch_case0 {
// COMMON-LOWERING-DAG:                 0 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     __switch_case0 = 1;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 1 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     __switch_case0 = 2;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 2 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 20;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     break '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 3 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 40;
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
// COMMON-REWRITES-DAG:     5 => {
// COMMON-REWRITES-DAG:         out += 10;
// COMMON-REWRITES-DAG:         let _v21: i32 = 20;
// COMMON-REWRITES-DAG:         let _v22: i32 = out;
// COMMON-REWRITES-DAG:         let _v23: i32 = _v22 + _v21;
// COMMON-REWRITES-DAG:         out = _v23;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     6 => {
// COMMON-REWRITES-DAG:         out += 20;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     7 => {
// COMMON-REWRITES-DAG:         out += 40;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     _ => {
// COMMON-REWRITES-DAG:         out += 1;
// COMMON-REWRITES-DAG:         let _v15: i32 = 10;
// COMMON-REWRITES-DAG:         let _v16: i32 = out;
// COMMON-REWRITES-DAG:         let _v17: i32 = _v16 + _v15;
// COMMON-REWRITES-DAG:         out = _v17;
// COMMON-REWRITES-DAG:         let _v18: i32 = 20;
// COMMON-REWRITES-DAG:         let _v19: i32 = out;
// COMMON-REWRITES-DAG:         let _v20: i32 = _v19 + _v18;
// COMMON-REWRITES-DAG:         out = _v20;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
