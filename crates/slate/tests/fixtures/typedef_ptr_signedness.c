#include <stdio.h>

typedef unsigned char my_char_t;

struct buffer {
  my_char_t *start;
  my_char_t *end;
};

int main(void) {
  my_char_t     data[4] = {65, 66, 67, 0};
  struct buffer b;
  b.start = data;
  b.end   = data + 3;
  printf("%c%c%c\n", b.start[0], b.start[1], b.start[2]);
  printf("%d\n", (int)(b.end - b.start));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct buffer {
// CHECK-NEXT:     start: *mut u8,
// CHECK-NEXT:     end: *mut u8,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut data: [u8; 4] = [65, 66, 67, 0];
// CHECK-NEXT:     let mut b: buffer = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     b.start = data.as_mut_ptr() as *mut u8;
// CHECK-NEXT:     b.end = unsafe { (data.as_mut_ptr() as *mut u8).offset(3 as isize) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%c%c%c\n".as_ptr(),
// CHECK-NEXT:             ((unsafe { *unsafe { b.start.offset(0 as isize) } }) as u32) as i32,
// CHECK-NEXT:             ((unsafe { *unsafe { b.start.offset(1 as isize) } }) as u32) as i32,
// CHECK-NEXT:             ((unsafe { *unsafe { b.start.offset(2 as isize) } }) as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", ((unsafe { b.end.offset_from(b.start as * mut u8) }) as i64) as i32);
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
