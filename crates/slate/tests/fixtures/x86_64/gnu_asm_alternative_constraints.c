#include <stdio.h>

// @check-fn-begin
static int add_ri(int a, int b) {
  int result;
  __asm__("addl %2, %0" : "=r"(result) : "0"(a), "ri"(b) : "cc");
  return result;
}
// @check-fn-end

// @check-fn-begin
static int add_rm(int a, int *p) {
  int result;
  __asm__("addl %2, %0" : "=r"(result) : "0"(a), "rm"(*p) : "cc");
  return result;
}
// @check-fn-end

int main(void) {
  int x = 4;
  printf("%d %d\n", add_ri(3, 5), add_rm(3, &x));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn add_ri(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl {1:e}, {0:e}", inlateout(reg) a as u32 => [[T_0]], in (reg) b as u32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn add_rm(mut a: i32, mut p: &i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl {1:e}, {0:e}", inlateout(reg) a as u32 => [[T_0]], in (reg) (unsafe { * (p
// CHECK-NEXT:             as * const i32) }) as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// SLATE-FILECHECK-END
