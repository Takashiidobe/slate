
#include <stdio.h>

static int volatile_local(int value) {
  volatile int slot = value;
  slot              = slot + 3;
  return slot;
}

int main(void) {
  printf("%d\n", volatile_local(4));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn volatile_local(mut slot: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(slot),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(slot)) + 3,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(slot)) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", volatile_local(4 as i32));
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
