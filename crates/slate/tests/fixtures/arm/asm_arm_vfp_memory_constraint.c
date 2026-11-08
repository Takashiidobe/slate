#include <stdio.h>

// @check-fn-begin
static int load_via_uv(const int *address) {
  int value;
  __asm__("ldr %0, %1" : "=r"(value) : "Uv"(*address));
  return value;
}
// @check-fn-end

int main(void) {
  int value = 42;
  printf("%d\n", load_via_uv(&value));
  return 0;
}

// SLATE-FILECHECK-BEGIN armv7-unknown-linux-gnueabihf
// CHECK-LABEL: {{^}}fn load_via_uv(mut address: &i32) -> i32 {
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "ldr {0}, [{1}]", lateout(reg) [[T_0]], in (reg) address as * const i32,
// CHECK-NEXT:             options(pure, readonly, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK: {{^}}}
// SLATE-FILECHECK-END
