#include <stdio.h>

enum Status { STATUS_OK, STATUS_FAIL };

typedef enum Status Processor(int x);

struct Dispatcher {
  Processor *run;
};

static int lastCode = -1;

static enum Status succeed(int x) {
  lastCode = x + 100;
  return STATUS_OK;
}

static enum Status fail(int x) {
  lastCode = x + 200;
  return STATUS_FAIL;
}

int main(void) {
  struct Dispatcher d;
  d.run = succeed;
  d.run(1);
  int a = lastCode;
  d.run = fail;
  d.run(2);
  int b = lastCode;
  printf("%d %d\n", a, b);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Dispatcher {
// CHECK-NEXT:     run: Option<unsafe extern "C-unwind" fn(i32) -> u32>,
// CHECK-NEXT: }
// CHECK-NEXT: static mut lastCode: i32 = -1;
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.succeed")]
// CHECK-NEXT: extern "C-unwind" fn succeed(x: i32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         lastCode = x + 100;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.fail")]
// CHECK-NEXT: extern "C-unwind" fn fail(x: i32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         lastCode = x + 200;
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: Dispatcher = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     d.run = Some(succeed);
// CHECK-NEXT:     unsafe { d.run.unwrap()(1i32) };
// CHECK-NEXT:     let a: i32 = unsafe { lastCode };
// CHECK-NEXT:     d.run = Some(fail);
// CHECK-NEXT:     unsafe { d.run.unwrap()(2i32) };
// CHECK-NEXT:     let b: i32 = unsafe { lastCode };
// CHECK-NEXT:     println!("{} {}", a, b);
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
