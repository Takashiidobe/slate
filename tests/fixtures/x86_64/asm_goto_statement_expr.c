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
// LOWERING-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 1;
// LOWERING-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-X86_64-GNU-DAG:             }
// LOWERING-X86_64-GNU-DAG:             1 => {
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: i32 = 11;
// LOWERING-X86_64-GNU-DAG:                 result2 = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: i32 = value;
// LOWERING-X86_64-GNU-DAG:                 unsafe {
// LOWERING-X86_64-GNU-DAG:                     core::arch::asm!("testl {0:e}, {0:e}\n\tjz {1}", in(reg) {{__v[0-9]+}}, label {
// LOWERING-X86_64-GNU-DAG: {{__state[0-9]+}} = 4;
// LOWERING-X86_64-GNU-DAG: continue '{{__dispatch[0-9]+}};
// LOWERING-X86_64-GNU-DAG: }, options(att_syntax));
// LOWERING-X86_64-GNU-DAG:                 }
// LOWERING-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 2;
// LOWERING-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-X86_64-GNU-DAG:             }
// LOWERING-X86_64-GNU-DAG:             2 => {
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-DAG:                 if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-DAG:                     {{__state[0-9]+}} = 3;
// LOWERING-X86_64-GNU-DAG:                 } else {
// LOWERING-X86_64-GNU-DAG:                     {{__state[0-9]+}} = 5;
// LOWERING-X86_64-GNU-DAG:                 }
// LOWERING-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-X86_64-GNU-DAG:             }
// LOWERING-X86_64-GNU-DAG:             3 => {
// LOWERING-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 4;
// LOWERING-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-X86_64-GNU-DAG:             }
// LOWERING-X86_64-GNU-DAG:             4 => {
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: i32 = 13;
// LOWERING-X86_64-GNU-DAG:                 result2 = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 5;
// LOWERING-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-X86_64-GNU-DAG:             }
// LOWERING-X86_64-GNU-DAG:             5 => {
// LOWERING-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 6;
// LOWERING-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-X86_64-GNU-DAG:             }
// LOWERING-X86_64-GNU-DAG:             6 => {
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: i32 = result2;
// LOWERING-X86_64-GNU-DAG:                 tmp = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 7;
// LOWERING-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-X86_64-GNU-DAG:             }
// LOWERING-X86_64-GNU-DAG:             7 => {
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: i32 = tmp;
// LOWERING-X86_64-GNU-DAG:                 result = {{__v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 1;
// REWRITES-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// REWRITES-X86_64-GNU-DAG:             }
// REWRITES-X86_64-GNU-DAG:             1 => {
// REWRITES-X86_64-GNU-DAG:                 result2 = 11;
// REWRITES-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: i32 = value;
// REWRITES-X86_64-GNU-DAG:                 unsafe {
// REWRITES-X86_64-GNU-DAG:                     core::arch::asm!("testl {0:e}, {0:e}\n\tjz {1}", in(reg) {{__v[0-9]+}}, label {
// REWRITES-X86_64-GNU-DAG: {{__state[0-9]+}} = 4;
// REWRITES-X86_64-GNU-DAG: continue '{{__dispatch[0-9]+}};
// REWRITES-X86_64-GNU-DAG: }, options(att_syntax));
// REWRITES-X86_64-GNU-DAG:                 }
// REWRITES-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 2;
// REWRITES-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// REWRITES-X86_64-GNU-DAG:             }
// REWRITES-X86_64-GNU-DAG:             2 => {
// REWRITES-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-DAG:                 if {{__v[0-9]+}} != 0 {
// REWRITES-X86_64-GNU-DAG:                     {{__state[0-9]+}} = 3;
// REWRITES-X86_64-GNU-DAG:                 } else {
// REWRITES-X86_64-GNU-DAG:                     {{__state[0-9]+}} = 5;
// REWRITES-X86_64-GNU-DAG:                 }
// REWRITES-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// REWRITES-X86_64-GNU-DAG:             }
// REWRITES-X86_64-GNU-DAG:             3 => {
// REWRITES-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 4;
// REWRITES-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// REWRITES-X86_64-GNU-DAG:             }
// REWRITES-X86_64-GNU-DAG:             4 => {
// REWRITES-X86_64-GNU-DAG:                 result2 = 13;
// REWRITES-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 5;
// REWRITES-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// REWRITES-X86_64-GNU-DAG:             }
// REWRITES-X86_64-GNU-DAG:             5 => {
// REWRITES-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 6;
// REWRITES-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// REWRITES-X86_64-GNU-DAG:             }
// REWRITES-X86_64-GNU-DAG:             6 => {
// REWRITES-X86_64-GNU-DAG:                 tmp = result2;
// REWRITES-X86_64-GNU-DAG:                 {{__state[0-9]+}} = 7;
// REWRITES-X86_64-GNU-DAG:                 continue '{{__dispatch[0-9]+}};
// REWRITES-X86_64-GNU-DAG:             }
// REWRITES-X86_64-GNU-DAG:             7 => {
// REWRITES-X86_64-GNU-DAG:                 result = tmp;
// SLATE-FILECHECK-END rewrites
