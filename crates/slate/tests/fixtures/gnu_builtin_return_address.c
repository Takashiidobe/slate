#include <stddef.h>
#include <stdio.h>

static int address_probe(void) {
  void *return_address = __builtin_return_address(0);
  void *frame_address  = __builtin_frame_address(0);
  return (return_address != NULL) + (frame_address != NULL);
}

int main(void) {
  printf("%d\n", address_probe());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.frameaddress.p0"]
// CHECK-NEXT:     fn __slate_intrinsic_2d7c29df8fdf6378([[ARG_0]]: i32) -> *mut ();
// CHECK-NEXT:     #[link_name = "llvm.returnaddress.p0"]
// CHECK-NEXT:     fn __slate_intrinsic_ded315013ea6523([[ARG_0]]: i32) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: fn address_probe() -> i32 {
// CHECK-NEXT:     let return_address: *mut () = unsafe {
// CHECK-NEXT:         __slate_intrinsic_ded315013ea6523(const { ((0 as i32) as u32) as i32 })
// CHECK-NEXT:     };
// CHECK-NEXT:     let frame_address: *mut () = unsafe {
// CHECK-NEXT:         __slate_intrinsic_2d7c29df8fdf6378(const { ((0 as i32) as u32) as i32 })
// CHECK-NEXT:     };
// CHECK-NEXT:     (!return_address.is_null() as i32) + (!frame_address.is_null() as i32)
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
