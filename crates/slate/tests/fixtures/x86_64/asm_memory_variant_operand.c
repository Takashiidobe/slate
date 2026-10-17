#include <stdio.h>

// @check-fn-begin
static int read_via_o(int x) {
  int result;
  __asm__("movl %1, %0" : "=r"(result) : "o"(x));
  return result;
}
// @check-fn-end

// @check-fn-begin
static int incr_via_V(int x) {
  __asm__("incl %0" : "+V"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static int store_via_o(void) {
  int x;
  __asm__("movl $42, %0" : "=o"(x));
  return x;
}
// @check-fn-end

int main(void) {
  printf("%d %d %d\n", read_via_o(7), incr_via_V(9), store_via_o());
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn read_via_o(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl ({1:r}), {0:e}", lateout(reg) [[T_0]], in (reg) std::ptr::addr_of!(x),
// CHECK-NEXT:             options(pure, readonly, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn incr_via_V(mut x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incl ({0:r})", in (reg) std::ptr::addr_of_mut!(x), options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn store_via_o() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl $42, ({0:r})", in (reg) std::ptr::addr_of_mut!(x), options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// SLATE-FILECHECK-END
