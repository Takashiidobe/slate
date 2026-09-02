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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = x;
// LOWERING-DAG: {
// LOWERING-DAG: let __switch_value0 = {{_v[0-9]+}};
// LOWERING-DAG: let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG: 1 => 0,
// LOWERING-DAG: 3 => 2,
// LOWERING-DAG: 4 => 3,
// LOWERING-DAG: _ => 1,
// LOWERING-DAG: };
// LOWERING-DAG: '__switch0: loop {
// LOWERING-DAG: match __switch_case0 {
// LOWERING-DAG: 0 => {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = out;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: out = {{_v[0-9]+}};
// LOWERING-DAG: __switch_case0 = 1;
// LOWERING-DAG: continue '__switch0;
// LOWERING-DAG: }
// LOWERING-DAG: 1 => {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = out;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: out = {{_v[0-9]+}};
// LOWERING-DAG: __switch_case0 = 2;
// LOWERING-DAG: continue '__switch0;
// LOWERING-DAG: }
// LOWERING-DAG: 2 => {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 3;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = out;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: out = {{_v[0-9]+}};
// LOWERING-DAG: break '__switch0;
// LOWERING-DAG: }
// LOWERING-DAG: 3 => {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 4;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = out;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: out = {{_v[0-9]+}};
// LOWERING-DAG: break '__switch0;
// LOWERING-DAG: }
// LOWERING-DAG: _ => {
// LOWERING-DAG: break '__switch0;
// LOWERING-DAG: }
// LOWERING-DAG: }
// LOWERING-DAG: }
// LOWERING-DAG: }
// LOWERING-DAG: }
// LOWERING-DAG: {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = x;
// LOWERING-DAG: {
// LOWERING-DAG: let __switch_value0 = {{_v[0-9]+}};
// LOWERING-DAG: let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-DAG: 2 => 0,
// LOWERING-DAG: 5 => 2,
// LOWERING-DAG: _ => 1,
// LOWERING-DAG: };
// LOWERING-DAG: '__switch0: loop {
// LOWERING-DAG: match __switch_case0 {
// LOWERING-DAG: 0 => {
// LOWERING-DAG: __switch_case0 = 1;
// LOWERING-DAG: continue '__switch0;
// LOWERING-DAG: }
// LOWERING-DAG: 1 => {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 10;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = out;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: out = {{_v[0-9]+}};
// LOWERING-DAG: __switch_case0 = 2;
// LOWERING-DAG: continue '__switch0;
// LOWERING-DAG: }
// LOWERING-DAG: 2 => {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 20;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = out;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: out = {{_v[0-9]+}};
// LOWERING-DAG: break '__switch0;
// LOWERING-DAG: }
// LOWERING-DAG: _ => {
// LOWERING-DAG: break '__switch0;
// LOWERING-DAG: }
// LOWERING-DAG: }
// LOWERING-DAG: }
// LOWERING-DAG: }
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: match x {
// REWRITES-DAG: 1 => {
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG: out = out + {{_v[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = out;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-DAG: out = {{_v[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = out;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-DAG: out = {{_v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: 3 => {
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = out;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-DAG: out = {{_v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: 4 => {
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-DAG: out = out + {{_v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: _ => {
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = out;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-DAG: out = {{_v[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = out;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-DAG: out = {{_v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: }
// REWRITES-DAG: match x {
// REWRITES-DAG: 5 => {
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 20;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = out;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-DAG: out = {{_v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: _ => {
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 10;
// REWRITES-DAG: out = out + {{_v[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 20;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = out;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-DAG: out = {{_v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
