#include <stdio.h>
#include <string.h>

int main(void) {
  char dst[16];
  char src[8] = "hello";
  memcpy(dst, src, 6);
  memset(dst + 5, 'A', 3);
  dst[8] = 0;
  char moved[16];
  memmove(moved, dst, 9);
  printf("%s\n", moved);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn memmove(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut dst: __SlateAlign16<[i8; 16]> = __SlateAlign16([0i8; 16]);
// CHECK-NEXT:     let mut src: [i8; 8] = [104, 101, 108, 108, 111, 0, 0, 0];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             src.as_mut_ptr() as *const u8,
// CHECK-NEXT:             dst.0.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             6 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             ((dst.0.as_mut_ptr() as *mut i8).add(5) as *mut ()) as *mut u8,
// CHECK-NEXT:             65u8,
// CHECK-NEXT:             3 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     dst.0[8] = 0;
// CHECK-NEXT:     let mut moved: __SlateAlign16<[i8; 16]> = __SlateAlign16([0i8; 16]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy(
// CHECK-NEXT:             dst.0.as_mut_ptr() as *const u8,
// CHECK-NEXT:             moved.0.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             9 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s\n".as_ptr(), moved.0.as_mut_ptr() as *mut i8) };
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
