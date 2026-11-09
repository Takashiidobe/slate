#include <stdio.h>

static void spin_pause(void) { __asm__ __volatile__("pause"); }

static void spin_pause_memory(void) { __asm__ __volatile__("pause" ::: "memory"); }

static void spin_rep_nop(void) { __asm__ __volatile__("rep; nop" ::: "memory"); }

int main(void) {
  spin_pause();
  spin_pause_memory();
  spin_rep_nop();
  printf("done\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn spin_pause() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("pause", options(att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn spin_pause_memory() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("pause", options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn spin_rep_nop() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("rep; nop", options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     spin_pause();
// CHECK-NEXT:     spin_pause_memory();
// CHECK-NEXT:     spin_rep_nop();
// CHECK-NEXT:     println!("done");
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
