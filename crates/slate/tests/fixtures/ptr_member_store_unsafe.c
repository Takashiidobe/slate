#include <stdio.h>

struct inner {
  int start;
  int end;
};

struct outer {
  struct inner buf;
  int          error;
};

static void bump(int *p) { *p = *p + 10; }

static void init(struct outer *o) {
  o->buf.start = 1;
  o->buf.end   = 2;
  o->error     = 0;
  bump(&o->buf.start);
}

int main(void) {
  struct outer o;
  init(&o);
  printf("%d %d %d\n", o.buf.start, o.buf.end, o.error);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct inner {
// CHECK-NEXT:     start: i32,
// CHECK-NEXT:     end: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct outer {
// CHECK-NEXT:     buf: inner,
// CHECK-NEXT:     error: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn bump(p: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = (unsafe { *p }) + 10;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn init(o: &mut outer) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(o as *mut outer)).buf.start = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(o as *mut outer)).buf.end = 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(o as *mut outer)).error = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     bump(unsafe { std::ptr::addr_of_mut!((* (o as * mut outer)).buf.start) });
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut o: outer = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     init(unsafe { &mut (*std::ptr::addr_of_mut!(o)) });
// CHECK-NEXT:     println!("{} {} {}", o.buf.start, o.buf.end, o.error);
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
