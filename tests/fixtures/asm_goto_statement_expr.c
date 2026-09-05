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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 1;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             1 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 11;
// COMMON-LOWERING-DAG:                 result2 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-DAG:                 unsafe {
// COMMON-LOWERING-DAG:                     core::arch::asm!("testl {0:e}, {0:e}\n\tjz {1}", in(reg) {{__v[0-9]+}}, label {
// COMMON-LOWERING-DAG: {{__state[0-9]+}} = 4;
// COMMON-LOWERING-DAG: continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG: }, options(att_syntax));
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 2;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             2 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-DAG:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 3;
// COMMON-LOWERING-DAG:                 } else {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 5;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             3 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 4;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             4 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 13;
// COMMON-LOWERING-DAG:                 result2 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 5;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             5 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 6;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             6 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = result2;
// COMMON-LOWERING-DAG:                 tmp = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 7;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             7 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = tmp;
// COMMON-LOWERING-DAG:                 result = {{__v[0-9]+}};
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG:                 {{__state[0-9]+}} = 1;
// COMMON-REWRITES-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             1 => {
// COMMON-REWRITES-DAG:                 result2 = 11;
// COMMON-REWRITES-DAG:                 let {{__v[0-9]+}}: i32 = value;
// COMMON-REWRITES-DAG:                 unsafe {
// COMMON-REWRITES-DAG:                     core::arch::asm!("testl {0:e}, {0:e}\n\tjz {1}", in(reg) {{__v[0-9]+}}, label {
// COMMON-REWRITES-DAG: {{__state[0-9]+}} = 4;
// COMMON-REWRITES-DAG: continue '{{__dispatch[0-9]+}};
// COMMON-REWRITES-DAG: }, options(att_syntax));
// COMMON-REWRITES-DAG:                 }
// COMMON-REWRITES-DAG:                 {{__state[0-9]+}} = 2;
// COMMON-REWRITES-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             2 => {
// COMMON-REWRITES-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-DAG:                 if {{__v[0-9]+}} != 0 {
// COMMON-REWRITES-DAG:                     {{__state[0-9]+}} = 3;
// COMMON-REWRITES-DAG:                 } else {
// COMMON-REWRITES-DAG:                     {{__state[0-9]+}} = 5;
// COMMON-REWRITES-DAG:                 }
// COMMON-REWRITES-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             3 => {
// COMMON-REWRITES-DAG:                 {{__state[0-9]+}} = 4;
// COMMON-REWRITES-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             4 => {
// COMMON-REWRITES-DAG:                 result2 = 13;
// COMMON-REWRITES-DAG:                 {{__state[0-9]+}} = 5;
// COMMON-REWRITES-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             5 => {
// COMMON-REWRITES-DAG:                 {{__state[0-9]+}} = 6;
// COMMON-REWRITES-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             6 => {
// COMMON-REWRITES-DAG:                 tmp = result2;
// COMMON-REWRITES-DAG:                 {{__state[0-9]+}} = 7;
// COMMON-REWRITES-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             7 => {
// COMMON-REWRITES-DAG:                 result = tmp;
// SLATE-FILECHECK-END common-rewrites
