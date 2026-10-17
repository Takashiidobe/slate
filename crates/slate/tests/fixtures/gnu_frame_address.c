#include <stdint.h>

static int frame_address_is_plausible(void) {
  void *fp = __builtin_frame_address(0);
  return (uintptr_t)fp > 0x1000;
}

int main(void) { return frame_address_is_plausible() ? 0 : 1; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.frameaddress.p0"]
// CHECK-NEXT:     fn __slate_intrinsic_2d7c29df8fdf6378([[ARG_0:arg[0-9]+]]: i32) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: fn frame_address_is_plausible() -> i32 {
// CHECK-NEXT:     let fp: *mut () = unsafe {
// CHECK-NEXT:         __slate_intrinsic_2d7c29df8fdf6378(const { ((0 as i32) as u32) as i32 })
// CHECK-NEXT:     };
// CHECK-NEXT:     ((fp as u64) > 4096) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if frame_address_is_plausible() != 0 { 0 } else { 1 }
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
