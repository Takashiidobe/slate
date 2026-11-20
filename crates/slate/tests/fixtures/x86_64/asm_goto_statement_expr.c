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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut result: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut [[RESULT_0:result_[0-9]+]]: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         {
// CHECK-NEXT:             [[RESULT_0]] = 0;
// CHECK-NEXT:             let mut __slate_asm_target: bool = false;
// CHECK-NEXT:             core::arch::asm!(
// CHECK-NEXT:                 "testl {0:e}, {0:e}\n\tjz {1}",
// CHECK-NEXT:                 in(reg) (0 as i32) as u32,
// CHECK-NEXT:                 label {
// CHECK-NEXT:                     __slate_asm_target = true;
// CHECK-NEXT:                 },
// CHECK-NEXT:                 options(nostack, att_syntax),
// CHECK-NEXT:             );
// CHECK-NEXT:             if __slate_asm_target {
// CHECK-NEXT:                 [[RESULT_0]] = 13;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 [[RESULT_0]] = 11;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_0]] = [[RESULT_0]];
// CHECK-NEXT:         result = 0;
// CHECK-NEXT:         let mut __slate_asm_target: bool = false;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "testl {0:e}, {0:e}\n\tjz {1}",
// CHECK-NEXT:             in(reg) (9 as i32) as u32,
// CHECK-NEXT:             label {
// CHECK-NEXT:                 __slate_asm_target = true;
// CHECK-NEXT:             },
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         if __slate_asm_target {
// CHECK-NEXT:             result = 13;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             result = 11;
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{} {}", [[V_0]], result);
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
