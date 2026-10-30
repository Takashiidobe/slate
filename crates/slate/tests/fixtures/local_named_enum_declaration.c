#include <stdio.h>

typedef enum { XML_STATUS_ERROR = 0, XML_STATUS_OK = 1 } XML_Status;

struct Case {
  XML_Status expectedStatus;
};

int main(void) {
  struct Case cases[2];
  cases[0].expectedStatus = XML_STATUS_OK;
  cases[1].expectedStatus = XML_STATUS_ERROR;

  for (int i = 0; i < 2; i++) {
    const XML_Status expectedStatus = cases[i].expectedStatus;
    if (expectedStatus == XML_STATUS_OK) {
      printf("ok\n");
    } else {
      printf("error\n");
    }
  }
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
// CHECK-NEXT: struct Case {
// CHECK-NEXT:     expectedStatus: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut cases: [Case; 2] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     cases[0].expectedStatus = 1;
// CHECK-NEXT:     cases[1].expectedStatus = 0;
// CHECK-NEXT:     for i in 0i32..2i32 {
// CHECK-NEXT:         let expectedStatus: u32 = cases[i as usize].expectedStatus;
// CHECK-NEXT:         if (expectedStatus as u32) == 1 {
// CHECK-NEXT:             println!("ok");
// CHECK-NEXT:         } else {
// CHECK-NEXT:             println!("error");
// CHECK-NEXT:         }
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
