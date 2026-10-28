#include <stdio.h>

// @check-fn-begin
static int atomic_increment(int *counter) {
  int old, status;
  __asm__ volatile("1: ldxr %w0, %1\n"
                    "\tadd %w0, %w0, #1\n"
                    "\tstxr %w3, %w0, %2\n"
                    "\tcbnz %w3, 1b"
                    : "=&r"(old), "+Q"(*counter), "+Q"(*counter), "=&r"(status));
  return old;
}
// @check-fn-end

int main(void) {
  int counter = 41;
  int result = atomic_increment(&counter);
  printf("%d %d\n", result, counter);
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn atomic_increment(mut counter: &i32) -> i32 {
// CHECK-NEXT:     let mut old: i32 = 0;
// CHECK-NEXT:     let mut status: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "1: ldxr {0:w}, [{1:x}]\n\tadd {0:w}, {0:w}, #1\n\tstxr {3:w}, {0:w}, [{2:x}]\n\tcbnz {3:w}, 1b",
// CHECK-NEXT:             out(reg) [[T_0]], in (reg) counter as * const i32, in (reg) counter as * const
// CHECK-NEXT:             i32, out(reg) [[T_1]], options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         old = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:         status = unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     old
// CHECK: {{^}}}
// SLATE-FILECHECK-END
