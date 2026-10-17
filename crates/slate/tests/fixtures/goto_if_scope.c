#include <stdio.h>

int main() {
  int n = 7;
  int cls;
  if (n < 0) {
    goto neg;
  } else {
    goto nonneg;
  }
neg:
  cls = -1;
  goto print;
nonneg:
  cls = 1;
  goto print;
print:
  printf("%d\n", cls);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut cls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let n: i32 = 7;
// CHECK-NEXT:         if n < 0 {
// CHECK-NEXT:             cls = -1;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             cls = 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{}", cls);
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
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
