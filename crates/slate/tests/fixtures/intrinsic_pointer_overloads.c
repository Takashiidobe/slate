#include <stdio.h>

int main(void) {
  void *first = __builtin_thread_pointer();
  void *second = __builtin_thread_pointer();
  unsigned char buffer[16] = {0};
  __builtin___clear_cache((char *)buffer, (char *)buffer + sizeof(buffer));
  printf("%d %d\n", first != 0, first == second);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.thread.pointer.p0"]
// CHECK-NEXT:     fn __slate_intrinsic_3b4650fdfadac1ac() -> *mut ();
// CHECK-NEXT:     #[link_name = "llvm.clear_cache"]
// CHECK-NEXT:     fn __slate_intrinsic_7eb885185d29c159([[ARG_0:arg[0-9]+]]: *mut (), [[ARG_1:arg[0-9]+]]: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let first: *mut () = unsafe { __slate_intrinsic_3b4650fdfadac1ac() };
// CHECK-NEXT:     let second: *mut () = unsafe { __slate_intrinsic_3b4650fdfadac1ac() };
// CHECK-NEXT:     let mut buffer: __SlateAlign16<[u8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [u8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0 as u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_intrinsic_7eb885185d29c159(
// CHECK-NEXT:             buffer.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             (buffer.0.as_mut_ptr() as *mut i8).offset(16 as isize) as *mut (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {}", ! first.is_null() as i32, (first == second) as i32);
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
