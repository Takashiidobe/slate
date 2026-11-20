
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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn volatile_local(mut slot: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write_volatile(
// CHECK-NEXT:             core::ptr::addr_of_mut!(slot),
// CHECK-NEXT:             core::ptr::read_volatile(core::ptr::addr_of!(slot)) + 3,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { core::ptr::read_volatile(core::ptr::addr_of!(slot)) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", volatile_local(4i32));
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
