#include <stdio.h>

#define CLASSIFY_ZERO(value)                                                   \
  ({                                                                           \
    __label__ zero, done;                                                      \
    int result = 0;                                                            \
    __asm__ goto("testl %0, %0\n\tjz %l[zero]" : : "r"(value) : "cc" : zero);  \
    result = 11;                                                               \
    goto done;                                                                 \
  zero:                                                                        \
    result = 13;                                                               \
  done:                                                                        \
    result;                                                                    \
  })

// @check-fn-begin
int main(void) {
  printf("%d %d\n", CLASSIFY_ZERO(0), CLASSIFY_ZERO(9));
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut result: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[RESULT_0:result_[0-9]+]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 6usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     {}
// CHECK-NEXT:                     println!("{} {}", [[V_0]], result);
// CHECK-NEXT:                     return 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     result = 13;
// CHECK-NEXT:                     __slate_state = 0usize;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     result = 11;
// CHECK-NEXT:                     __slate_state = 0usize;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     {}
// CHECK-NEXT:                     [[V_0]] = [[RESULT_0]];
// CHECK-NEXT:                     result = 0;
// CHECK-NEXT:                     __slate_state = 2usize;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::arch::asm!(
// CHECK-NEXT:                             "testl {0:e}, {0:e}\n\tjz {1}", in (reg) (9 as i32) as u32,
// CHECK-NEXT:                             label { __slate_state = 1usize; }, options(nostack,
// CHECK-NEXT:                             att_syntax)
// CHECK-NEXT:                         );
// CHECK-NEXT:                     }
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     [[RESULT_0]] = 13;
// CHECK-NEXT:                     __slate_state = 3usize;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     [[RESULT_0]] = 11;
// CHECK-NEXT:                     __slate_state = 3usize;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     [[RESULT_0]] = 0;
// CHECK-NEXT:                     __slate_state = 5usize;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::arch::asm!(
// CHECK-NEXT:                             "testl {0:e}, {0:e}\n\tjz {1}", in (reg) (0 as i32) as u32,
// CHECK-NEXT:                             label { __slate_state = 4usize; }, options(nostack,
// CHECK-NEXT:                             att_syntax)
// CHECK-NEXT:                         );
// CHECK-NEXT:                     }
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
