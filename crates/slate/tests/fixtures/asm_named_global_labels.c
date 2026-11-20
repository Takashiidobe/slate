#include <stdio.h>

extern const char slate_cs_start[], slate_cs_end[];

__attribute__((noinline)) static int bump(int x) {
  __asm__ volatile("slate_cs_start:\n\tnop\nslate_cs_end:" ::: "memory");
  return x + 1;
}

int main(void) {
  printf("%d %d\n", bump(1), slate_cs_end >= slate_cs_start);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![allow(named_asm_labels)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut slate_cs_start: [i8; 0];
// CHECK-NEXT:     static mut slate_cs_end: [i8; 0];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[inline(never)]
// CHECK-NEXT: fn bump(x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "slate_cs_start:\n\tnop\nslate_cs_end:",
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         bump(1i32),
// CHECK-NEXT:         ((unsafe { core::ptr::addr_of!(slate_cs_end) as *const i8 })
// CHECK-NEXT:             >= unsafe { core::ptr::addr_of!(slate_cs_start) as *const i8 }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
