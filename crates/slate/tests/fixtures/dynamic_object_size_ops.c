#include <stdio.h>

static int global_array[4];

int main(void) {
  int local[6];

  unsigned long local_whole     = __builtin_dynamic_object_size(local, 0);
  unsigned long local_remaining = __builtin_dynamic_object_size(&local[2], 1);
  unsigned long global_whole    = __builtin_dynamic_object_size(global_array, 0);
  unsigned long global_lower    = __builtin_dynamic_object_size(&global_array[3], 3);

  printf("%lu %lu %lu %lu\n", local_whole, local_remaining, global_whole,
         global_lower);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut global_array: __SlateAlign16<[i32; 4]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     __SlateAlign16([0i32; 6]);
// CHECK-NEXT:     let local_whole: u64 = 24;
// CHECK-NEXT:     let local_remaining: u64 = 16;
// CHECK-NEXT:     let global_whole: u64 = 16;
// CHECK-NEXT:     let global_lower: u64 = 4;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             local_whole,
// CHECK-NEXT:             local_remaining,
// CHECK-NEXT:             global_whole,
// CHECK-NEXT:             global_lower,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
