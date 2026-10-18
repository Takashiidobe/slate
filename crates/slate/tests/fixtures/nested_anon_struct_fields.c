#include <stdio.h>

struct event {
  int type;
  union {
    struct {
      char *value;
    } alias;
    struct {
      char *handle;
      char *suffix;
    } tag;
  } data;
};

int main(void) {
  struct event e;
  e.type            = 1;
  char h[]          = "H";
  char s[]          = "S";
  e.data.tag.handle = h;
  e.data.tag.suffix = s;
  printf("%d %s%s\n", e.type, e.data.tag.handle, e.data.tag.suffix);
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
// CHECK-NEXT: struct event {
// CHECK-NEXT:     r#type: i32,
// CHECK-NEXT:     data: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0]] {
// CHECK-NEXT:     alias: [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT:     tag: [[SLATERECORD_2:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1]] {
// CHECK-NEXT:     value: *mut i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_2]] {
// CHECK-NEXT:     handle: *mut i8,
// CHECK-NEXT:     suffix: *mut i8,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut e: event = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     e.r#type = 1;
// CHECK-NEXT:     let mut h: [i8; 2] = [72, 0];
// CHECK-NEXT:     let mut s: [i8; 2] = [83, 0];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e.data.tag.handle = h.as_mut_ptr() as *mut i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e.data.tag.suffix = s.as_mut_ptr() as *mut i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%d %s%s\n".as_ptr(), e.r#type, e.data.tag.handle, e.data.tag.suffix)
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
