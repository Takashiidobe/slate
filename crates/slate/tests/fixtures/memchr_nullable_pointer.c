#include <stdio.h>

int main(void) {
  unsigned char  buf[4] = {10, 20, 30, 40};
  unsigned char *hit    = (unsigned char *)__builtin_memchr(buf, 30, 4);
  unsigned char *miss   = (unsigned char *)__builtin_memchr(buf, 99, 4);
  printf("%ld %d\n", hit - buf, miss == 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     #[link_name = "memchr"]
// CHECK-NEXT:     fn __builtin_memchr([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [u8; 4] = [10, 20, 30, 40];
// CHECK-NEXT:     let hit: *mut u8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(buf.as_mut_ptr() as *const (), 30 as i32, 4 as u64)
// CHECK-NEXT:     }) as *mut u8;
// CHECK-NEXT:     let miss: *mut u8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(buf.as_mut_ptr() as *const (), 99 as i32, 4 as u64)
// CHECK-NEXT:     }) as *mut u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld %d\n".as_ptr(),
// CHECK-NEXT:             hit.offset_from(buf.as_mut_ptr() as *mut u8) as i64,
// CHECK-NEXT:             miss.is_null() as i32,
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
