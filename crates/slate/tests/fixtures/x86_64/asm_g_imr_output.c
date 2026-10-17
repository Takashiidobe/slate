#include <stdio.h>

// @check-fn-begin
static int store_via_g(void) {
  int x;
  __asm__("movl $42, %0" : "=g"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static int store_via_imr(void) {
  int x;
  __asm__("movl $7, %0" : "=imr"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static int add_tied_g(int y) {
  int x = 1;
  __asm__("addl %1, %0" : "+g"(x) : "r"(y));
  return x;
}
// @check-fn-end

int main(void) {
  printf("%d %d %d\n", store_via_g(), store_via_imr(), add_tied_g(4));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn store_via_g() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl $42, {0:e}", lateout(reg) [[T_0]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn store_via_imr() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl $7, {0:e}", lateout(reg) [[T_0]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn add_tied_g(mut y: i32) -> i32 {
// CHECK-NEXT:     let mut x: i32 = 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl {1:e}, {0:e}", inlateout(reg) x as u32 => [[T_0]], in (reg) y as u32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// SLATE-FILECHECK-END
