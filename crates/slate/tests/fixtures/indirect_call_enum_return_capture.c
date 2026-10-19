#include <stdio.h>

typedef enum { E_OK = 0, E_FAIL = 1 } Status;

typedef Status (*Handler)(int);

struct Parser {
  Handler processor;
};

static Status handle_ok(int x) {
  (void)x;
  return E_OK;
}

static Status handle_fail(int x) {
  (void)x;
  return E_FAIL;
}

int main(void) {
  struct Parser p;
  for (int i = 0; i < 2; i++) {
    p.processor   = (i == 0) ? handle_ok : handle_fail;
    Status result = p.processor(5);
    if (result == E_OK) {
      printf("ok\n");
    } else {
      printf("fail\n");
    }
  }
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
// CHECK-NEXT: struct Parser {
// CHECK-NEXT:     processor: Option<unsafe extern "C-unwind" fn(i32) -> u32>,
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.handle_ok")]
// CHECK-NEXT: extern "C-unwind" fn handle_ok(x: i32) -> u32 {
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.handle_fail")]
// CHECK-NEXT: extern "C-unwind" fn handle_fail(x: i32) -> u32 {
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: Parser = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < 2 {
// CHECK-NEXT:         p.processor = {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: Option<unsafe extern "C-unwind" fn(i32) -> u32> = if i == 0 {
// CHECK-NEXT:                 Some(handle_ok)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 Some(handle_fail)
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         };
// CHECK-NEXT:         let result: u32 = unsafe { p.processor.unwrap()(5i32) };
// CHECK-NEXT:         if (result as u32) == 0 {
// CHECK-NEXT:             println!("ok");
// CHECK-NEXT:         } else {
// CHECK-NEXT:             println!("fail");
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
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
