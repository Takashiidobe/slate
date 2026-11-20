#include <stdio.h>

static int global_value = 42;

// @check-fn-begin
static int read_via_address(void) {
  int result;
  __asm__("movl %a1, %0" : "=r"(result) : "p"(&global_value));
  return result;
}
// @check-fn-end

int main(void) {
  printf("%d\n", read_via_address());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn read_via_address() -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl ({1:r}), {0:e}",
// CHECK-NEXT:             lateout(reg) [[T_0]],
// CHECK-NEXT:             in(reg) core::ptr::addr_of_mut!(global_value) as u64,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { core::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// SLATE-FILECHECK-END
