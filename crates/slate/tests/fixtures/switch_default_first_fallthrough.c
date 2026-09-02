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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = x;
// LOWERING-DAG:     {
// LOWERING-DAG:         let __switch_value0 = {{_v[0-9]+}};
// LOWERING-DAG:         let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG:             5 => 1,
// LOWERING-DAG:             6 => 2,
// LOWERING-DAG:             7 => 3,
// LOWERING-DAG:             _ => 0,
// LOWERING-DAG:         };
// LOWERING-DAG:         '__switch0: loop {
// LOWERING-DAG:             match __switch_case0 {
// LOWERING-DAG:                 0 => {
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                     out = {{_v[0-9]+}};
// LOWERING-DAG:                     __switch_case0 = 1;
// LOWERING-DAG:                     continue '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 1 => {
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                     out = {{_v[0-9]+}};
// LOWERING-DAG:                     __switch_case0 = 2;
// LOWERING-DAG:                     continue '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 2 => {
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = 20;
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                     out = {{_v[0-9]+}};
// LOWERING-DAG:                     break '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 3 => {
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = 40;
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                     out = {{_v[0-9]+}};
// LOWERING-DAG:                     break '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 _ => {
// LOWERING-DAG:                     break '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: match x {
// REWRITES-DAG:     5 => {
// REWRITES-DAG:         out += 10;
// REWRITES-DAG:         out += 20;
// REWRITES-DAG:     }
// REWRITES-DAG:     6 => {
// REWRITES-DAG:         out += 20;
// REWRITES-DAG:     }
// REWRITES-DAG:     7 => {
// REWRITES-DAG:         out += 40;
// REWRITES-DAG:     }
// REWRITES-DAG:     _ => {
// REWRITES-DAG:         out += 1;
// REWRITES-DAG:         out += 10;
// REWRITES-DAG:         out += 20;
// REWRITES-DAG:     }
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
