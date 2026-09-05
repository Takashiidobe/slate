// { dg-do run { target x86_64-*-* } }
#include <stdio.h>

#define CLASSIFY_ZERO(value)                                                   \
  ({                                                                           \
    __label__ zero;                                                            \
    int result = 11;                                                           \
    __asm__ goto("testl %0, %0\n\tjz %l[zero]" : : "r"(value) : "cc" : zero);  \
    if (0) {                                                                   \
    zero:                                                                      \
      result = 13;                                                             \
    }                                                                          \
    result;                                                                    \
  })

static int classify_zero(int value) {
  int result;
  // @lowering-begin
  // @rewrite-begin
  result = CLASSIFY_ZERO(value);
  // @rewrite-end
  // @lowering-end
  return result;
}

int main(void) {
  printf("%d %d\n", classify_zero(0), classify_zero(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG:     {{__state[0-9]+}} = 1;
// LOWERING-DAG:     continue '{{__dispatch[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: 1 => {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 11;
// LOWERING-DAG:     result2 = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = value;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         core::arch::asm!("testl {0:e}, {0:e}\n\tjz {1}", in(reg) {{_v[0-9]+}}, label {
// LOWERING-DAG:             {{__state[0-9]+}} = 4;
// LOWERING-DAG:             continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:         }, options(att_syntax));
// LOWERING-DAG:     }
// LOWERING-DAG:     {{__state[0-9]+}} = 2;
// LOWERING-DAG:     continue '{{__dispatch[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: 2 => {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-DAG:     if {{_v[0-9]+}} {
// LOWERING-DAG:         {{__state[0-9]+}} = 3;
// LOWERING-DAG:     } else {
// LOWERING-DAG:         {{__state[0-9]+}} = 5;
// LOWERING-DAG:     }
// LOWERING-DAG:     continue '{{__dispatch[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: 3 => {
// LOWERING-DAG:     {{__state[0-9]+}} = 4;
// LOWERING-DAG:     continue '{{__dispatch[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: 4 => {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 13;
// LOWERING-DAG:     result2 = {{_v[0-9]+}};
// LOWERING-DAG:     {{__state[0-9]+}} = 5;
// LOWERING-DAG:     continue '{{__dispatch[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: 5 => {
// LOWERING-DAG:     {{__state[0-9]+}} = 6;
// LOWERING-DAG:     continue '{{__dispatch[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: 6 => {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = result2;
// LOWERING-DAG:     tmp = {{_v[0-9]+}};
// LOWERING-DAG:     {{__state[0-9]+}} = 7;
// LOWERING-DAG:     continue '{{__dispatch[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: 7 => {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = tmp;
// LOWERING-DAG:     result = {{_v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG:     {{__state[0-9]+}} = 1;
// REWRITES-DAG:     continue '{{__dispatch[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: 1 => {
// REWRITES-DAG:     result2 = 11;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = value;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         core::arch::asm!("testl {0:e}, {0:e}\n\tjz {1}", in(reg) {{_v[0-9]+}}, label {
// REWRITES-DAG:                                 {{__state[0-9]+}} = 4;
// REWRITES-DAG:                                 continue '{{__dispatch[0-9]+}};
// REWRITES-DAG:                             }, options(att_syntax));
// REWRITES-DAG:     }
// REWRITES-DAG:     {{__state[0-9]+}} = 2;
// REWRITES-DAG:     continue '{{__dispatch[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: 2 => {
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     if {{_v[0-9]+}} != 0 {
// REWRITES-DAG:         {{__state[0-9]+}} = 3;
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__state[0-9]+}} = 5;
// REWRITES-DAG:     }
// REWRITES-DAG:     continue '{{__dispatch[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: 3 => {
// REWRITES-DAG:     {{__state[0-9]+}} = 4;
// REWRITES-DAG:     continue '{{__dispatch[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: 4 => {
// REWRITES-DAG:     result2 = 13;
// REWRITES-DAG:     {{__state[0-9]+}} = 5;
// REWRITES-DAG:     continue '{{__dispatch[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: 5 => {
// REWRITES-DAG:     {{__state[0-9]+}} = 6;
// REWRITES-DAG:     continue '{{__dispatch[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: 6 => {
// REWRITES-DAG:     tmp = result2;
// REWRITES-DAG:     {{__state[0-9]+}} = 7;
// REWRITES-DAG:     continue '{{__dispatch[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: 7 => {
// REWRITES-DAG:     result = tmp;
// SLATE-FILECHECK-END rewrites
