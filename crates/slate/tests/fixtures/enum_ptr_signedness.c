#include <stdio.h>

typedef enum color_e { RED, GREEN, BLUE } color_t;

struct palette {
  color_t *start;
  color_t *top;
};

int main(void) {
  color_t        data[3] = {RED, GREEN, BLUE};
  struct palette p;
  p.start   = data;
  p.top     = data + 3;
  color_t c = *(p.start + 1);
  printf("%d\n", (int)c);
  printf("%d\n", (int)(p.top - p.start));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct palette {
// CHECK-NEXT:     start: *mut u32,
// CHECK-NEXT:     top: *mut u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut data: [u32; 3] = [0, 1, 2];
// CHECK-NEXT:     let mut p: palette = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     p.start = data.as_mut_ptr() as *mut u32;
// CHECK-NEXT:     p.top = unsafe { (data.as_mut_ptr() as *mut u32).add(3) };
// CHECK-NEXT:     let c: u32 = unsafe { *p.start.add(1) };
// CHECK-NEXT:     println!("{}", (c as u32) as i32);
// CHECK-NEXT:     println!("{}", ((unsafe { p.top.offset_from(p.start as * mut u32) }) as i64) as i32);
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
