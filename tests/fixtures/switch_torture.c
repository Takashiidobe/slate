#include <stdio.h>

int f(int x) {
  int out = 0;
  // @lowering-begin
  // @rewrite-begin
  switch (x) {
  case -500 ... -1:
    out += 100;
  case 0:
  case 1:
    out += 1;
  case 2 ... 100:
    out += 2;
    if (out > 50) {
      break;
    }
    out += 3;
  case 101:
    out += 4;
    if (x % 2 == 0) {
      break;
    }
  default:
    out += 5;
  case 200 ... 500:
  case 600:
  case 700 ... 900:
    out += 6;
    break;
  case 999:
    out += 7;
  case 1000:
    out += 8;
  }
  // @rewrite-end
  // @lowering-end
  return out;
}

int main(void) {
  for (int x = -600; x < 1001; x++)
    printf("%d,", f(x));
  printf("\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:     {
// LOWERING-DAG:         let __switch_value0 = {{__v[0-9]+}};
// LOWERING-DAG:         let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG:             -500..=-1 => 0,
// LOWERING-DAG:             0 => 1,
// LOWERING-DAG:             1 => 2,
// LOWERING-DAG:             2..=100 => 3,
// LOWERING-DAG:             101 => 4,
// LOWERING-DAG:             200..=500 => 6,
// LOWERING-DAG:             600 => 7,
// LOWERING-DAG:             700..=900 => 8,
// LOWERING-DAG:             999 => 9,
// LOWERING-DAG:             1000 => 10,
// LOWERING-DAG:             _ => 5,
// LOWERING-DAG:         };
// LOWERING-DAG:         '__switch0: loop {
// LOWERING-DAG:             match __switch_case0 {
// LOWERING-DAG:                 0 => {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 100;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                     out = {{__v[0-9]+}};
// LOWERING-DAG:                     __switch_case0 = 1;
// LOWERING-DAG:                     continue '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 1 => {
// LOWERING-DAG:                     __switch_case0 = 2;
// LOWERING-DAG:                     continue '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 2 => {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                     out = {{__v[0-9]+}};
// LOWERING-DAG:                     __switch_case0 = 3;
// LOWERING-DAG:                     continue '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 3 => {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                     out = {{__v[0-9]+}};
// LOWERING-DAG:                     {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = out;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 50;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-DAG:                         if {{__v[0-9]+}} {
// LOWERING-DAG:                             break '__switch0;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                     out = {{__v[0-9]+}};
// LOWERING-DAG:                     __switch_case0 = 4;
// LOWERING-DAG:                     continue '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 4 => {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                     out = {{__v[0-9]+}};
// LOWERING-DAG:                     {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} % {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:                         if {{__v[0-9]+}} {
// LOWERING-DAG:                             break '__switch0;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                     __switch_case0 = 5;
// LOWERING-DAG:                     continue '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 5 => {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                     out = {{__v[0-9]+}};
// LOWERING-DAG:                     __switch_case0 = 6;
// LOWERING-DAG:                     continue '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 6 => {
// LOWERING-DAG:                     __switch_case0 = 7;
// LOWERING-DAG:                     continue '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 7 => {
// LOWERING-DAG:                     __switch_case0 = 8;
// LOWERING-DAG:                     continue '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 8 => {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 6;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                     out = {{__v[0-9]+}};
// LOWERING-DAG:                     break '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 9 => {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                     out = {{__v[0-9]+}};
// LOWERING-DAG:                     __switch_case0 = 10;
// LOWERING-DAG:                     continue '__switch0;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 10 => {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 8;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                     out = {{__v[0-9]+}};
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
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = x;
// REWRITES-DAG: let __switch_value0 = {{__v[0-9]+}};
// REWRITES-DAG: let mut __switch_case0: i32 = match __switch_value0 {
// REWRITES-DAG:     -500..=-1 => 0,
// REWRITES-DAG:     0 => 1,
// REWRITES-DAG:     1 => 2,
// REWRITES-DAG:     2..=100 => 3,
// REWRITES-DAG:     101 => 4,
// REWRITES-DAG:     200..=500 => 6,
// REWRITES-DAG:     600 => 7,
// REWRITES-DAG:     700..=900 => 8,
// REWRITES-DAG:     999 => 9,
// REWRITES-DAG:     1000 => 10,
// REWRITES-DAG:     _ => 5,
// REWRITES-DAG: };
// REWRITES-DAG: '__switch0: loop {
// REWRITES-DAG:     match __switch_case0 {
// REWRITES-DAG:         0 => {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 100;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = out;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             out = {{__v[0-9]+}};
// REWRITES-DAG:             __switch_case0 = 1;
// REWRITES-DAG:             continue '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:         1 => {
// REWRITES-DAG:             __switch_case0 = 2;
// REWRITES-DAG:             continue '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:         2 => {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = out;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             out = {{__v[0-9]+}};
// REWRITES-DAG:             __switch_case0 = 3;
// REWRITES-DAG:             continue '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:         3 => {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = out;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             out = {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = out;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 50;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// REWRITES-DAG:             if {{__v[0-9]+}} {
// REWRITES-DAG:                 break '__switch0;
// REWRITES-DAG:             }
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = out;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             out = {{__v[0-9]+}};
// REWRITES-DAG:             __switch_case0 = 4;
// REWRITES-DAG:             continue '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:         4 => {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 4;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = out;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             out = {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = x;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} % {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:             if {{__v[0-9]+}} {
// REWRITES-DAG:                 break '__switch0;
// REWRITES-DAG:             }
// REWRITES-DAG:             __switch_case0 = 5;
// REWRITES-DAG:             continue '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:         5 => {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 5;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = out;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             out = {{__v[0-9]+}};
// REWRITES-DAG:             __switch_case0 = 6;
// REWRITES-DAG:             continue '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:         6 => {
// REWRITES-DAG:             __switch_case0 = 7;
// REWRITES-DAG:             continue '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:         7 => {
// REWRITES-DAG:             __switch_case0 = 8;
// REWRITES-DAG:             continue '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:         8 => {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 6;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = out;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             out = {{__v[0-9]+}};
// REWRITES-DAG:             break '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:         9 => {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 7;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = out;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             out = {{__v[0-9]+}};
// REWRITES-DAG:             __switch_case0 = 10;
// REWRITES-DAG:             continue '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:         10 => {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 8;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = out;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             out = {{__v[0-9]+}};
// REWRITES-DAG:             break '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:         _ => {
// REWRITES-DAG:             break '__switch0;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
