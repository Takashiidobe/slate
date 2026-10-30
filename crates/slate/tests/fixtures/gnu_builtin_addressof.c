#include <stdio.h>

static int address_probe(void) {
  int  value   = 37;
  int *address = __builtin_addressof(value);
  return (address == &value) + (*address == 37);
}

int main(void) {
  printf("%d\n", address_probe());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn address_probe() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 37;
// CHECK-NEXT:     let address: *mut i32 = std::ptr::addr_of_mut!(value);
// CHECK-NEXT:     ((address == std::ptr::addr_of_mut!(value)) as i32)
// CHECK-NEXT:         + (((unsafe { *address }) == 37) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", address_probe());
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
