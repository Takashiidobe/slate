#include <stdio.h>

int score(int x) {
  int out = 0;
  // @lowering-begin
  // @rewrite-begin
  switch (x) {
  case 1:
    out += 1;
  default:
    out += 2;
  case 3:
    out += 3;
    break;
  case 4:
    out += 4;
  }
  // @rewrite-end
  // @lowering-end
  return out;
}

int shared(int x) {
  int out = 0;
  // @lowering-begin
  // @rewrite-begin
  switch (x) {
  case 2:
  default:
    out += 10;
  case 5:
    out += 20;
  }
  // @rewrite-end
  // @lowering-end
  return out;
}

int main(void) {
  for (int i = 0; i < 7; i++)
    printf("%d,%d ", score(i), shared(i));
  printf("\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-DAG:             1 => 0,
// COMMON-LOWERING-DAG:             3 => 2,
// COMMON-LOWERING-DAG:             4 => 3,
// COMMON-LOWERING-DAG:             _ => 1,
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
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     __switch_case0 = 2;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 2 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     break '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 3 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 4;
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
// COMMON-LOWERING-DAG: {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-DAG:             2 => 0,
// COMMON-LOWERING-DAG:             5 => 2,
// COMMON-LOWERING-DAG:             _ => 1,
// COMMON-LOWERING-DAG:         };
// COMMON-LOWERING-DAG:         '__switch0: loop {
// COMMON-LOWERING-DAG:             match __switch_case0 {
// COMMON-LOWERING-DAG:                 0 => {
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
// COMMON-REWRITES-DAG:         out += 1;
// COMMON-REWRITES-DAG:         let _v15: i32 = 2;
// COMMON-REWRITES-DAG:         let _v16: i32 = out;
// COMMON-REWRITES-DAG:         let _v17: i32 = _v16 + _v15;
// COMMON-REWRITES-DAG:         out = _v17;
// COMMON-REWRITES-DAG:         let _v18: i32 = 3;
// COMMON-REWRITES-DAG:         let _v19: i32 = out;
// COMMON-REWRITES-DAG:         let _v20: i32 = _v19 + _v18;
// COMMON-REWRITES-DAG:         out = _v20;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     3 => {
// COMMON-REWRITES-DAG:         out += 3;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     4 => {
// COMMON-REWRITES-DAG:         out += 4;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     _ => {
// COMMON-REWRITES-DAG:         out += 2;
// COMMON-REWRITES-DAG:         let _v21: i32 = 3;
// COMMON-REWRITES-DAG:         let _v22: i32 = out;
// COMMON-REWRITES-DAG:         let _v23: i32 = _v22 + _v21;
// COMMON-REWRITES-DAG:         out = _v23;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: match {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:     5 => {
// COMMON-REWRITES-DAG:         out += 20;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     _ => {
// COMMON-REWRITES-DAG:         out += 10;
// COMMON-REWRITES-DAG:         let _v9: i32 = 20;
// COMMON-REWRITES-DAG:         let _v10: i32 = out;
// COMMON-REWRITES-DAG:         let _v11: i32 = _v10 + _v9;
// COMMON-REWRITES-DAG:         out = _v11;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
