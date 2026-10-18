#include <stdio.h>

typedef struct {
  int *start;
  int *end;
  int *pointer;
} buffer_t;

int main(void) {
  struct {
    int x;
    int y;
  } point = {3, 4};

  int      storage[4];
  buffer_t buf = {0, 0, 0};
  buf.start    = storage;
  buf.pointer  = storage;
  buf.end      = storage + 4;

  *buf.pointer = point.x + point.y;
  buf.pointer++;
  *buf.pointer = point.x * point.y;
  buf.pointer++;

  printf("%d %d\n", storage[0], storage[1]);
  printf("%ld\n", (long)(buf.pointer - buf.start));
  printf("%ld\n", (long)(buf.end - buf.start));
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     start: *mut i32,
// CHECK-NEXT:     end: *mut i32,
// CHECK-NEXT:     pointer: *mut i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let point: [[SLATERECORD_1]] = [[SLATERECORD_1]] { x: 3, y: 4 };
// CHECK-NEXT:     let mut storage: __SlateAlign16<[i32; 4]> = __SlateAlign16([0 as i32; 4]);
// CHECK-NEXT:     let mut buf: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:         start: std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         end: std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         pointer: std::ptr::null_mut::<i32>(),
// CHECK-NEXT:     };
// CHECK-NEXT:     buf.start = storage.0.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     buf.pointer = storage.0.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     buf.end = unsafe { (storage.0.as_mut_ptr() as *mut i32).offset(4 as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *buf.pointer = point.x + point.y;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut i32 = buf.pointer;
// CHECK-NEXT:     buf.pointer = unsafe { [[V_0]].offset(1 as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *buf.pointer = point.x * point.y;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut i32 = buf.pointer;
// CHECK-NEXT:     buf.pointer = unsafe { [[V_1]].offset(1 as isize) };
// CHECK-NEXT:     println!("{} {}", storage.0[0], storage.0[1]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%ld\n".as_ptr(), buf.pointer.offset_from(buf.start as *mut i32) as i64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%ld\n".as_ptr(), buf.end.offset_from(buf.start as *mut i32) as i64)
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
